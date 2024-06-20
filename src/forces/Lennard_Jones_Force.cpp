#include "Lennard_Jones_Force.h"
#include <chrono>
#include <iostream>
#include <cmath>
    

Lennard_Jones_Force::Lennard_Jones_Force(std::array<bool,6> reflectLenJonesFlag, std::array<bool,3> periodicFlag) {
  this->reflectLenJonesFlag = reflectLenJonesFlag;
  this->periodicFlag = periodicFlag;
  spdlog::info("Lennard_Jones_Force object constructed");
};

Lennard_Jones_Force::~Lennard_Jones_Force() {
  spdlog::info("Lennard_Jones_Force object destructed");
};

void Lennard_Jones_Force::calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) {

  std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs = particles.getParticlePairs();
  
  // reset the force for each particle, store the old force and claculate the GravitationalForce
  for(auto particle : particles.getParticles()){
    particle->setOldF(particle->getF());
    std::array<double,3> gravForce = {0, particle->getM()*gravConstant,0};
    particle->setF(gravForce);
  }
  
  if(linkedcells){
    ParticleContainerLinkedCell &LCContainer = dynamic_cast<ParticleContainerLinkedCell&>(particles);
    double twoRoot6 = pow(2, 1/6);
    std::vector<std::shared_ptr<Particle>> boundery = LCContainer.getBoundary();
    
    for(int i = 0; i < 2; i++){
      for(int j = 0; j < 2; j++){
        for(int k = 0; k < 2; k++){
          if(i+j+k==0||(i==1&&!periodicFlag[0])||(j==1&&!periodicFlag[1])||(k==1&&!periodicFlag[2])) continue;
          auto pairs = LCContainer.getParticlePairsPeriodic({i==1,j==1,k==1});
          for(auto pair : pairs){
            auto particle_i = pair[0];
            auto particle_j = pair[1];
            double epsilon = sqrt(particle_i->getEpsilon()*particle_j->getEpsilon());
            double sigma = (particle_i->getSigma()+particle_j->getSigma())/2;

            int directions = 1;
            std::vector<std::array<double, 3>> direction = {particle_i->getX()-particle_j->getX()};
            for(auto d = 0; d < 3; d++){
              if((d==0&&i==1)||(d==1&&j==1)||(d==2&&k==1)){
                if(LCContainer.getCellCount()[d]==1){
                  for (int dir = 0; dir < directions; dir++) {
                    direction.push_back(direction[dir]);
                    direction[dir][d] -= LCContainer.getSize()[d];
                    direction[dir+directions][d] += LCContainer.getSize()[d];
                  }
                  directions *=2;
                }
                else{
                  for(int dir = 0; dir < directions; dir++){
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
  calculateFPairs(particlePairs);
}

void Lennard_Jones_Force::calculateFPairs(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs){
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

std::array<double,3> Lennard_Jones_Force::calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadius){
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
