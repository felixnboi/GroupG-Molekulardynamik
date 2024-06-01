#include "Lennard_Jones_Force.h"

Lennard_Jones_Force::Lennard_Jones_Force() {
  spdlog::info("Lennard_Jones_Force object constructed");
};
Lennard_Jones_Force::~Lennard_Jones_Force() {
  spdlog::info("Lennard_Jones_Force object destructed");
};

void Lennard_Jones_Force::calculateF(ParticleContainer &particles) {
  std::vector<Particle>::iterator particle_i; ///< Iterator for iterating over particles.
  std::vector<Particle>::iterator particle_j; ///< Second iterator for nested loop over particles.
  double epsilon = 5;
  double sigma = 1;
  double sigmaPow6 = pow(sigma,6);
  double sigmaPow12 = pow(sigma,12);
  // reset the force for each particle and store the old force
  for (particle_i = particles.begin(); particle_i != particles.end(); particle_i++){
    particle_i->setOldF(particle_i->getF());
    particle_i->setF({0,0,0});
  }

  // iterate over all pairs of particles to calculate forces
  for (particle_i = particles.begin(); particle_i != particles.end(); particle_i++) {
    auto cur_x_i = particle_i->getX();

    std::array<double, 3> cur_F_i_dummy = particle_i->getF();

     // inner loop to calculate force between particle i and all particles j after i respectfully
    for (particle_j = std::next(particle_i); particle_j!=particles.end(); particle_j++) {
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
}
