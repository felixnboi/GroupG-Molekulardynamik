#include "Lennard_Jones_Force.h"
#include <chrono>
#include <iostream>
    

Lennard_Jones_Force::Lennard_Jones_Force() {
  spdlog::info("Lennard_Jones_Force object constructed");
};
Lennard_Jones_Force::~Lennard_Jones_Force() {
  spdlog::info("Lennard_Jones_Force object destructed");
};

void Lennard_Jones_Force::calculateF(ParticleContainer &particles, std::array<bool,6> reflectLenJonesFlag) {
  std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs = particles.getParticlePairs();
  std::shared_ptr<Particle> particle_i;
  std::shared_ptr<Particle> particle_j;
  double epsilon = 5;
  double sigma = 1;
  double sigmaPow6 = pow(sigma,6);
  double sigmaPow12 = pow(sigma,12);
  // reset the force for each particle and store the old force
  for (auto pair = particlePairs.begin(); pair != particlePairs.end(); pair++){
  std::shared_ptr<Particle> particle = (*pair)[0];
    particle->setOldF(particle->getF());
    particle->setF({0,0,0});
  }
  if(reflectLenJonesFlag[0]||reflectLenJonesFlag[1]||reflectLenJonesFlag[2]||reflectLenJonesFlag[3]||reflectLenJonesFlag[4]||reflectLenJonesFlag[5])  {
    ParticleContainerLinkedCell &LCContainer = dynamic_cast<ParticleContainerLinkedCell&>(particles);
    std::vector<std::shared_ptr<Particle>> boundery = LCContainer.getBoundary();
    for(auto particle = boundery.begin(); particle != boundery.end(); particle++){
      std::array<double, 3> cur_F_dummy = (*particle)->getF();
      for(int i = 0; i < 3; i++){
        if(reflectLenJonesFlag[2*i]||(*particle)->getX()[i]< LCContainer.getCellSize()[i]){
          auto norm_squared = pow(2*(*particle)->getX()[i], 2);
          auto norm_pow6 = pow(norm_squared, 3);

          double directionlessForce = -24*epsilon/norm_squared*(sigmaPow6/norm_pow6-2*sigmaPow12/pow(norm_pow6,2));
          if(directionlessForce>0){
            cur_F_dummy[i] += directionlessForce *2*(*particle)->getX()[i];
          }
        }
        if(reflectLenJonesFlag[2*i+1]||(*particle)->getX()[i]> LCContainer.getSize()[i]-LCContainer.getCellSize()[i]){
          auto norm_squared = pow(2*(*particle)->getX()[i], 2);
          auto norm_pow6 = pow(norm_squared, 3);

          double directionlessForce = -24*epsilon/norm_squared*(sigmaPow6/norm_pow6-2*sigmaPow12/pow(norm_pow6,2));

          if(directionlessForce>0){
            cur_F_dummy[i] += directionlessForce *2*((*particle)->getX()[i]-LCContainer.getSize()[i]);
          }
        }
      }
      (*particle)->setF(cur_F_dummy);
    }
  }

  // iterate over all pairs of particles to calculate forces
  for (auto pair = particlePairs.begin(); pair != particlePairs.end(); pair++){
    particle_i = (*pair)[0];
    particle_j = (*pair)[1];
    auto cur_x_i = particle_i->getX();

    std::array<double, 3> cur_F_i_dummy = particle_i->getF();

    // inner loop to calculate force between particle i and all particles j after i respectfully
    auto cur_x_j = particle_j->getX();
    std::array<double, 3> cur_F_j_dummy = particle_j->getF();

    // calculating the Euclidean distance between particle i and particle j
    auto norm = ArrayUtils::L2Norm(cur_x_i - cur_x_j);
    auto norm_squared = pow(norm, 2);
    auto norm_pow6 = pow(norm_squared, 3);

    // calculating everything for the force except for the direction
    double directionlessForce = -24*epsilon/norm_squared*(sigmaPow6/norm_pow6-2*sigmaPow12/pow(norm_pow6,2));

    // calculating the force components (along the x, y, z axes) between particle i and particle j
    for(int k = 0; k<3; k++){
      double force = directionlessForce*(cur_x_i[k]-cur_x_j[k]);  
      cur_F_i_dummy[k] += force;
      cur_F_j_dummy[k] -= force;
    }
    // update the force for particle i and particle j
    particle_i->setF(cur_F_i_dummy);
    particle_j->setF(cur_F_j_dummy);
  }
}
