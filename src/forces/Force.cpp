#include "Force.h"
#include <chrono>
#include <iostream>
#include <cmath>
    

Force::Force(std::array<bool, 6> reflectLenJonesFlag, std::array<bool, 3> periodicFlag, bool lenJonesFlag, bool gravFlag, bool linkedcells, 
std::array<double, 3> gravConstant, bool membraneFlag, double k, double r0, size_t strategy) 
  : reflectLenJonesFlag(reflectLenJonesFlag),
    periodicFlag(periodicFlag),
    lenJonesFlag(lenJonesFlag),
    gravFlag(gravFlag),
    linkedcells(linkedcells),
    gravConstant(gravConstant),
    membraneFlag(membraneFlag),
    k(k),
    r0(r0),
    strategy(strategy),
    twoRoot6(pow(2, 1/6)) {}

Force::~Force() {}

void Force::calculateF(ParticleContainer &particles) {

  auto particlePairs = particles.getParticlePairs();
  
  // reset the force for each particle, store the old force and claculate the GravitationalForce
  for(const auto& particle : particles.getParticles()){
    particle->setOldF(particle->getF());
    particle->setF(gravConstant);
  }
  
  if(linkedcells){
    ParticleContainerLinkedCell &LCContainer = dynamic_cast<ParticleContainerLinkedCell&>(particles);
    calculateFPeriodic(LCContainer);    
    calculateFReflecting(LCContainer);
    if(membraneFlag) calculateFHarmonic(LCContainer, k, r0);
  }

  if(lenJonesFlag) calculateFLennardJones(particlePairs);
  if(gravFlag) calculateFGravitation(particlePairs);
}

void Force::calculateFPeriodic(ParticleContainerLinkedCell &LCContainer){
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 2; j++){
      for(int k = 0; k < 2; k++){
        // If i, j or k is set we view their respective boundery as periodic.
        if(i+j+k==0||(i==1&&!periodicFlag[0])||(j==1&&!periodicFlag[1])||(k==1&&!periodicFlag[2])) continue;
        // We filter out the cases where we view a boundery as periodic, but it isn't, because that would lead to false calculations.
        // We also filter out the case where we view none of them as peridic, since we handle that case else where.
        // Now the force is calculated for all particle pairs, which are connected through ALL the bouderies we view as being peridic.
        auto pairs = LCContainer.getParticlePairsPeriodic({i==1,j==1,k==1});
        if(strategy == 0){
          for(const auto& pair : pairs){
          calculateFPeriodicHelper(pair, i, j, k, LCContainer);
          }
        }
        if(strategy == 1 || strategy == 2 || strategy == 3){
          #ifdef _OPENMP
          #pragma omp parallel for schedule(auto)
          #endif
          for(const auto& pair : pairs){
            calculateFPeriodicHelper(pair, i, j, k, LCContainer);
          }
        }
      }
    }
  }
}

inline void Force::calculateFPeriodicHelper(const std::pair<Particle* const, Particle* const> &pair, int i, int j, int k, ParticleContainerLinkedCell &LCContainer){
  auto particle_i = pair.first;
  auto particle_j = pair.second;

  if (particle_i->getIsOuter() && particle_j->getIsOuter()) return;

  double epsilon = particle_i->getRootEpsilon()*particle_j->getRootEpsilon();
  double sigma = (particle_i->getSigma()+particle_j->getSigma())/2;
  
  std::vector<std::array<double, 3>> direction = {particle_i->getX()-particle_j->getX()}; // The dircetion/distance from one particle to the other. 
  // There can be multible possible directions because of the edge case 4 lines below this.
  int directions = 1; // The amount of arrays in the vector above, normally always one exept for the egde case 3 lines below this.
  for(auto d = 0; d < 3; d++){
    if((d==0&&i==1)||(d==1&&j==1)||(d==2&&k==1)){ // Is true if we view the boundery of the dimesion d as periodic.
      if(LCContainer.getCellCount()[d]==1){ // This is only for the edge case, where in one direction the domain is only one cell wide and the periodic boundery is set.
        // A two particle now might interact mutlible times, through the boundery in both directions and that per dimesion. (This case should porbably be avoided anyway.)
        for (int dir = 0; dir < directions; dir++) {
          direction.push_back(direction[dir]); // Doubles the amount of dircetions because for each the particles now can interact twice.
          direction[dir][d] -= LCContainer.getSize()[d]; // Sets the dircetion/distance form one patritcle to the other through one boundery.
          direction[dir+directions][d] += LCContainer.getSize()[d]; // Sets the dircetion/distance form one patritcle to the other through the other boundery.
          // This does not cancles out in the force calculation becasue there still can be sidewards movement.
        }
        directions *=2; // Upadetes the amount of dircetions.
      }
      else{
        for(int dir = 0; dir < directions; dir++){
          // The dircetion is updated to not be the difference of the actual position of the particles, but the way it would be through the periodic boundery.
          if(direction[dir][d]<0) direction[dir][d] += LCContainer.getSize()[d];
          else direction[dir][d] -= LCContainer.getSize()[d];
        }
      }
    }
  }
  for(int dir = 0; dir < directions; dir++){
    auto force = calculateLennardJonesForce(direction[dir], epsilon, sigma*sigma, LCContainer.getRadiusSquared());
    particle_i->applyF(force, strategy);
    particle_j->applyF(-1*force, strategy);
  }
}

void Force::calculateFReflecting(ParticleContainerLinkedCell &LCContainer){
  auto boundery = LCContainer.getBoundaries(reflectLenJonesFlag);

  if(strategy == 0){
    for(const auto& particle : boundery){
    calculateFReflectingHelper(particle, LCContainer);
    }
  }
  if(strategy == 1 || strategy == 2 || strategy == 3){
    #ifdef _OPENMP
    #pragma omp parallel for schedule(auto)
    #endif
    for(const auto& particle : boundery){
      calculateFReflectingHelper(particle, LCContainer);
    }
  }
}

inline void Force::calculateFReflectingHelper(Particle* particle, ParticleContainerLinkedCell &LCContainer){
  std::array<double, 3> force = {0,0,0};
  auto sigma  = particle->getSigma();
  for(int i = 0; i < 3; i++){
    auto xi = particle->getX()[i];
    if(reflectLenJonesFlag[2*i]&&xi< LCContainer.getCellSize()[i]&&xi< sigma*twoRoot6){
      std::array<double, 3> direction = {0,0,0};
      direction[i] = 2*xi;
      force[i] = calculateLennardJonesForce(direction, particle->getEpsilon(), sigma,0)[i];
    }
    if(reflectLenJonesFlag[2*i+1]&&xi> LCContainer.getSize()[i]-LCContainer.getCellSize()[i]&&xi> LCContainer.getSize()[i]-sigma*twoRoot6){
      std::array<double, 3> direction = {0,0,0};
      direction[i] = 2*(xi-LCContainer.getSize()[i]);
      force[i] = calculateLennardJonesForce(direction, particle->getEpsilon(), sigma,0)[i];
    }
  }
  particle->applyF(force, strategy);
}

void Force::calculateFLennardJones(std::vector<std::pair<Particle* const, Particle* const>> pairs){
  // iterate over all pairs of particles to calculate forces
  if(strategy == 0){
    for (const auto& pair : pairs){
      calculateFLennardJonesHelper(pair);
    }
  }
  if(strategy == 1 || strategy == 2 || strategy == 3){
    #ifdef _OPENMP
    #pragma omp parallel for schedule(auto)
    #endif
    for (const auto& pair : pairs){
      calculateFLennardJonesHelper(pair);
    }
  }
}

inline void Force::calculateFLennardJonesHelper(const std::pair<Particle* const, Particle* const> &pair){
  auto particle_i = pair.first;
  auto particle_j = pair.second;
  double epsilon = particle_i->getRootEpsilon()*particle_j->getRootEpsilon();
  double sigma = (particle_i->getSigma()+particle_j->getSigma())/2;
  double cutOffRadius = 0;

  if(membraneFlag) cutOffRadius = twoRoot6 * sigma;

  auto force = calculateLennardJonesForce(particle_i->getX()-particle_j->getX(), epsilon, sigma, cutOffRadius*cutOffRadius);
    
  particle_i->applyF(force, strategy);
  particle_j->applyF(-1*force, strategy);
}

void Force::calculateFGravitation(std::vector<std::pair<Particle* const, Particle* const>> pairs){
  for (auto& pair : pairs){
    auto particle_i = pair.first;
    auto particle_j = pair.second;

    auto distance = particle_j->getX() - particle_i->getX();

    auto force = particle_i->getM() * particle_j->getM()/pow(ArrayUtils::L2Norm(distance), 3) * distance;

    particle_i->applyF(force, strategy);
    particle_j->applyF(-1*force, strategy);
  }
}

std::array<double,3> Force::calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadiusSquared){
  // calculating the Euclidean distance between particle i and particle j
    auto normSquared = direction[0]*direction[0]+direction[1]*direction[1]+direction[2]*direction[2];
    if(cutOffRadiusSquared > 0 && normSquared > cutOffRadiusSquared) return {0,0,0};
    auto sigmaDividedByNormSquared = sigma*sigma/normSquared;
    auto sigmaDivedByNormPow6 = sigmaDividedByNormSquared*sigmaDividedByNormSquared*sigmaDividedByNormSquared;

    // calculating everything for the force except for the direction
    double directionlessForce = 48*epsilon/normSquared*sigmaDivedByNormPow6*(sigmaDivedByNormPow6-0.5);

    return directionlessForce*direction;
}

void Force::calculateFHarmonic(ParticleContainerLinkedCell &LCContainer, double k, double r0){
  double r0Diagonal = pow(2, 0.5) * r0;
  for(auto& particle : LCContainer.getParticles()){
    auto neighbours = particle->getNeighbours();
    auto hasNeighbour = particle->getHasNeighbour();

    if(hasNeighbour[0]) calculateHarmonicFroce(particle, neighbours[0], k, r0);
    if(hasNeighbour[1]) calculateHarmonicFroce(particle, neighbours[1], k, r0Diagonal);
    if(hasNeighbour[2]) calculateHarmonicFroce(particle, neighbours[2], k, r0);
    if(hasNeighbour[3]) calculateHarmonicFroce(particle, neighbours[3], k, r0Diagonal);
  }
}

void Force::calculateHarmonicFroce(Particle* particle1, Particle* particle2, double k, double r0){
  std::array<double,3> direction = particle2->getX() - particle1->getX();
  auto norm = ArrayUtils::L2Norm(direction);
  std::array<double,3> force = k*(norm-r0) / norm*direction;
  particle1->setF(particle1->getF()+force);
  particle2->setF(particle2->getF()-force);
}
