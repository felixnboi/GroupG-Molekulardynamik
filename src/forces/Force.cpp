#include "Force.h"
#include <chrono>
#include <iostream>
#include <cmath>
    

Force::Force(std::array<bool,6> reflectLenJonesFlag, std::array<bool,3> periodicFlag, bool lenJonesFlag, bool gravFlag, bool harmonicFlag, bool linkedcells, 
         double gravConstant, double k, double r0) {
  this->reflectLenJonesFlag = reflectLenJonesFlag;
  this->periodicFlag = periodicFlag;
  this-> lenJonesFlag = lenJonesFlag;
  this-> gravFlag = gravFlag;
  this-> harmonicFlag = harmonicFlag;
  this-> linkedcells = linkedcells;
  this-> gravConstant = gravConstant;
  this-> k = k;
  this-> r0 = r0;
  spdlog::info("Force object constructed");
};

Force::~Force() {
  spdlog::info("Force object destructed");
};

void Force::calculateF(ParticleContainer &particles) {

  std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs = particles.getParticlePairs();
  
  // reset the force for each particle, store the old force and claculate the GravitationalForce
  for(auto particle : particles.getParticles()){
    particle->setOldF(particle->getF());
    std::array<double,3> gravForce = {0, particle->getM()*gravConstant,0};
    particle->setF(gravForce);
  }
  
  if(linkedcells){
    ParticleContainerLinkedCell &LCContainer = dynamic_cast<ParticleContainerLinkedCell&>(particles);
    calculateFPeriodic(LCContainer);    
    calculateFReflecting(LCContainer);
    if(harmonicFlag) calculateFHarmonic(LCContainer, k, r0);
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
          for(auto pair : pairs){
            auto particle_i = pair[0];
            auto particle_j = pair[1];
            double epsilon = sqrt(particle_i->getEpsilon()*particle_j->getEpsilon());
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
              auto force = calculateLennardJonesForce(direction[dir], epsilon, sigma, LCContainer.getRadius());
              particle_i->setF(particle_i->getF()+force);
              particle_j->setF(particle_j->getF()-force);
            }
          }
        }
      }
    }
}

void Force::calculateFReflecting(ParticleContainerLinkedCell &LCContainer){
    double twoRoot6 = pow(2, 1/6);
    std::vector<std::shared_ptr<Particle>> boundery = LCContainer.getBoundary();

    for(auto particle : boundery){
      std::array<double, 3> force = {0,0,0};
      for(int i = 0; i < 3; i++){
        if(reflectLenJonesFlag[2*i]&&particle->getX()[i]< LCContainer.getCellSize()[i]&&particle->getX()[i]< particle->getSigma()*twoRoot6){
          std::array<double, 3> direction = {0,0,0};
          direction[i] = 2*particle->getX()[i];
          force[i] = calculateLennardJonesForce(direction, particle->getEpsilon(), particle->getSigma(),0)[i];
        }
        if(reflectLenJonesFlag[2*i+1]&&particle->getX()[i]> LCContainer.getSize()[i]-LCContainer.getCellSize()[i]&&particle->getX()[i]> LCContainer.getSize()[i]-particle->getSigma()*twoRoot6){
          std::array<double, 3> direction = {0,0,0};
          direction[i] = 2*(particle->getX()[i]-LCContainer.getSize()[i]);
          force[i] = calculateLennardJonesForce(direction, particle->getEpsilon(), particle->getSigma(),0)[i];
        }
      }
      particle->applyF(force);
    }
}

void Force::calculateFLennardJones(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs){
  // iterate over all pairs of particles to calculate forces
  for (auto pair = pairs.begin(); pair != pairs.end(); pair++){
    std::shared_ptr<Particle> particle_i = (*pair)[0];
    std::shared_ptr<Particle> particle_j = (*pair)[1];
    double epsilon = sqrt(particle_i->getEpsilon()*particle_j->getEpsilon());
    double sigma = (particle_i->getSigma()+particle_j->getSigma())/2;

    auto force = calculateLennardJonesForce(particle_i->getX()-particle_j->getX(), epsilon, sigma, 0);
     
    particle_i->setF(particle_i->getF()+force);
    particle_j->setF(particle_j->getF()-force);
  }
}

void Force::calculateFGravitation(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs){
  for (auto pair : pairs){
    auto particle_i = pair[0];
    auto particle_j = pair[1];

    auto distance = particle_j->getX() - particle_i->getX();

    auto force = particle_i->getM() * particle_j->getM()/pow(ArrayUtils::L2Norm(distance), 3) * distance;

    particle_i->setF(particle_i->getF()+force);
    particle_j->setF(particle_j->getF()-force);
  }
}

std::array<double,3> Force::calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadius){
  // calculating the Euclidean distance between particle i and particle j
    auto norm = ArrayUtils::L2Norm(direction);
    if(cutOffRadius > 0 && norm > cutOffRadius) return {0,0,0};
    auto norm_squared = pow(norm, 2);
    auto norm_pow6 = pow(norm_squared, 3);
    auto sigmaPow6 = pow(sigma,6);

    // calculating everything for the force except for the direction
    double directionlessForce = -24*epsilon/norm_squared*(sigmaPow6/norm_pow6-2*pow(sigmaPow6/norm_pow6,2));

    // calculating the force components (along the x, y, z axes) between particle i and particle j
    std::array<double,3> force = directionlessForce*direction;
    
    return force;
}

void Force::calculateFHarmonic(ParticleContainerLinkedCell &LCContainer, double k, double r0){
  double r0Diagonal = pow(2, 0.5) * r0;
  for(auto particle : LCContainer.getParticles()){
    auto neighbours = particle->getNeighbours();
    auto hasNeighbour = particle->getHasNeighbour();

    if(hasNeighbour[0]) calculateHarmonicFroce(particle, neighbours[0], k, r0);
    if(hasNeighbour[1]) calculateHarmonicFroce(particle, neighbours[1], k, r0Diagonal);
    if(hasNeighbour[2]) calculateHarmonicFroce(particle, neighbours[2], k, r0);
    if(hasNeighbour[3]) calculateHarmonicFroce(particle, neighbours[3], k, r0Diagonal);
  }
}

void Force::calculateHarmonicFroce(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, double k, double r0){
  std::array<double,3> direction = particle2->getX() -particle1->getX();
  auto norm = ArrayUtils::L2Norm(direction);
  std::array<double,3> force = k*(norm-r0)/norm*direction;
  particle1->setF(particle1->getF()+force);
  particle2->setF(particle2->getF()-force);
}
