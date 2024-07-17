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
    r0Diagonal(pow(2, 0.5) * r0),
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
    if(membraneFlag) calculateFHarmonic(LCContainer);
  }

  if(lenJonesFlag) calculateFLennardJones(particlePairs);
  if(gravFlag) calculateFGravitation(particlePairs);
}

void Force::calculateFLennardJones(std::vector<std::pair<Particle*, Particle*>> pairs){
  // Depending on the strategy selceted the next for loop is paralellized or not. In the loop we interate over all pairs ofparticles
  // in pairs (all particles that are closer than the cut off radius). For each particle pair we call calculateFLennardJonesOnePair, which calculates the
  // Lennard Johnes force they apply to each other.
  if(strategy == 0){
    for (const auto& pair : pairs){
      calculateFLennardJonesOnePair(pair);
    }
  }
  else{ // strategy is 1, 2 or 3
    #ifdef _OPENMP
    #pragma omp parallel for schedule(static, 4)
    #endif
    for (const auto& pair : pairs){
      calculateFLennardJonesOnePair(pair);
    }
  }
}

inline void Force::calculateFLennardJonesOnePair(const std::pair<Particle *, Particle *> &pair){
  auto particle_i = pair.first;
  auto particle_j = pair.second;
  double epsilon = particle_i->getRootEpsilon()*particle_j->getRootEpsilon();
  double sigma = particle_i->getSigmaHalf()+particle_j->getSigmaHalf();
  double cutOffRadius = 0;

  if(membraneFlag) cutOffRadius = twoRoot6 * sigma;

  auto force = lennardJonesForceFormula(particle_i->getX()-particle_j->getX(), epsilon, sigma, cutOffRadius*cutOffRadius);
    
  particle_i->applyF(force);
  particle_j->applyF(-1*force);
}

void Force::calculateFGravitation(std::vector<std::pair<Particle*, Particle*>> pairs){
  for (auto& pair : pairs){
    auto particle_i = pair.first;
    auto particle_j = pair.second;

    auto distance = particle_j->getX() - particle_i->getX();

    auto force = particle_i->getM() * particle_j->getM()/pow(ArrayUtils::L2Norm(distance), 3) * distance;

    particle_i->setF(particle_i->getF()+force);
    particle_j->setF(particle_j->getF()-force);
  }
}

void Force::calculateFPeriodic(ParticleContainerLinkedCell &LCContainer){
  for (int tmp = 0; tmp < 8; ++tmp) {
    bool i = tmp & 1; // If true we view the x boundery as periodic.
    bool j = (tmp & 2) != 0; // If true we view the y boundery as periodic.
    bool k = (tmp & 4) != 0; // If true we view the z boundery as periodic.
    if(!(i||j||k)||(i&&!periodicFlag[0])||(j&&!periodicFlag[1])||(k&&!periodicFlag[2])) continue;
    // We filter out the cases where we view a boundery as periodic, but it isn't, because that would lead to false calculations.
    // We also filter out the case where we view none of them as peridic, since we handle that case else where.
    // Now the force is calculated for all particle pairs, which are connected through ALL the bouderies we view as being peridic.
    auto pairs = LCContainer.getParticlePairsPeriodic({i,j,k});

    // Depending on the strategy selceted the next for loop is paralellized or not. In the loop we interate over all particle pairs
    // in pairs. For each pair we call calculateFPeriodicOnePair, which calculates the Lennard Johens force for this pair.
    if(strategy == 0){
      for(const auto& pair : pairs){
      calculateFPeriodicOnePair(pair, i, j, k, LCContainer);
      }
    }
    else{ // strategy is 1, 2 or 3
      #ifdef _OPENMP
      #pragma omp parallel for schedule(static , 4)
      #endif
      for(const auto& pair : pairs){
        calculateFPeriodicOnePair(pair, i, j, k, LCContainer);
      }
    }
  }
}

inline void Force::calculateFPeriodicOnePair(const std::pair<Particle *, Particle *> &pair, bool i, bool j, bool k, ParticleContainerLinkedCell &LCContainer){
  auto particle_i = pair.first;
  auto particle_j = pair.second;

  if (particle_i->getIsOuter() && particle_j->getIsOuter()) return;

  double epsilon = particle_i->getRootEpsilon()*particle_j->getRootEpsilon();
  double sigma = particle_i->getSigmaHalf()+particle_j->getSigmaHalf();
  
  std::vector<std::array<double, 3>> direction = {particle_i->getX()-particle_j->getX()}; // The dircetion/distance from one particle to the other. 
  // There can be multible possible directions because of the edge case 4 lines below this.
  int directions = 1; // The amount of arrays in the vector above, normally always one exept for the egde case 3 lines below this.
  for(auto d = 0; d < 3; d++){
    if((d==0&&i)||(d==1&&j)||(d==2&&k)){ // Is true if we view the boundery of the dimesion d as periodic.
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
    auto force = lennardJonesForceFormula(direction[dir], epsilon, sigma*sigma, LCContainer.getRadiusSquared());
    particle_i->applyF(force);
    particle_j->applyF(-1*force);
  }
}

void Force::calculateFReflecting(ParticleContainerLinkedCell &LCContainer){
  std::vector<Particle*> boundery = LCContainer.getBoundaries(reflectLenJonesFlag);

  // Depending on the strategy selceted the next for loop is paralellized or not. In the loop we interate over all particles
  // in the boundery. For each particle we call calculateFReflectingOneParticle, which calculates the reflecting force applied to add particle.
  if(strategy == 0){
    for(const auto& particle : boundery){
    calculateFReflectingOneParticle(particle, LCContainer);
    }
  }
  if(strategy == 1 || strategy == 2 || strategy == 3){
    #ifdef _OPENMP
    #pragma omp parallel for schedule(static, 4)
    #endif
    for(const auto& particle : boundery){
      calculateFReflectingOneParticle(particle, LCContainer);
    }
  }
}

inline void Force::calculateFReflectingOneParticle(Particle* particle, ParticleContainerLinkedCell &LCContainer){
  std::array<double, 3> force = {0,0,0};
  auto sigma  = particle->getSigma();
  for(int i = 0; i < 3; i++){ // Itterates over each dimesion and applies the force if the particle is to close to the boundery there.
    auto xi = particle->getX()[i];
    if(reflectLenJonesFlag[2*i]&&xi< LCContainer.getCellSize()[i]&&xi< sigma*twoRoot6){
      std::array<double, 3> direction = {0,0,0};
      direction[i] = 2*xi;
      force[i] = lennardJonesForceFormula(direction, particle->getEpsilon(), sigma,0)[i];
    }
    if(reflectLenJonesFlag[2*i+1]&&xi> LCContainer.getSize()[i]-LCContainer.getCellSize()[i]&&xi> LCContainer.getSize()[i]-sigma*twoRoot6){
      std::array<double, 3> direction = {0,0,0};
      direction[i] = 2*(xi-LCContainer.getSize()[i]);
      force[i] = lennardJonesForceFormula(direction, particle->getEpsilon(), sigma,0)[i];
    }
  }
  particle->applyF(force);
}

std::array<double,3> Force::lennardJonesForceFormula(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadiusSquared){
    auto normSquared = direction[0]*direction[0]+direction[1]*direction[1]+direction[2]*direction[2];

    // In some cases it wasn't checkt if the particles are closer than the cut off radius, so we check in those cases.
    if(cutOffRadiusSquared > 0 && normSquared > cutOffRadiusSquared) return {0,0,0};
    auto sigmaDividedByNormSquared = sigma*sigma/normSquared;
    auto sigmaDivedByNormPow6 = sigmaDividedByNormSquared*sigmaDividedByNormSquared*sigmaDividedByNormSquared;

    // calculating everything for the force except for the direction
    double directionlessForce = 48*epsilon/normSquared*sigmaDivedByNormPow6*(sigmaDivedByNormPow6-0.5);

    return directionlessForce*direction;
}

void Force::calculateFHarmonic(ParticleContainerLinkedCell &LCContainer){
  for(auto& particle : LCContainer.getParticles()){
    auto neighbours = particle->getNeighbours();
    auto hasNeighbour = particle->getHasNeighbour();

    if(hasNeighbour[0]) harmonicFroceFormula(particle, neighbours[0],r0);
    if(hasNeighbour[1]) harmonicFroceFormula(particle, neighbours[1], r0Diagonal);
    if(hasNeighbour[2]) harmonicFroceFormula(particle, neighbours[2], r0);
    if(hasNeighbour[3]) harmonicFroceFormula(particle, neighbours[3], r0Diagonal);
  }
}

void Force::harmonicFroceFormula(Particle* particle1, Particle* particle2, double r0){
  std::array<double,3> direction = particle2->getX() - particle1->getX();
  auto norm = ArrayUtils::L2Norm(direction);
  std::array<double,3> force = k*(norm-r0) / norm*direction;
  particle1->setF(particle1->getF()+force);
  particle2->setF(particle2->getF()-force);
}
