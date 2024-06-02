#include "Gravitational_Force.h"

Gravitational_Force::Gravitational_Force() {
  spdlog::info("GravitationalForce object constructed");
};
Gravitational_Force::~Gravitational_Force() {
  spdlog::info("GravitationalForce object destructed");
};


void Gravitational_Force::calculateF(ParticleContainer &particles) {
  std::vector<Particle>::iterator particle_i; ///< Iterator for iterating over particles.
  std::vector<Particle>::iterator particle_j; ///< Second iterator for nested loop over particles.
  // reset the force for each particle and store the old force
  for (particle_i = particles.begin(); particle_i != particles.end(); particle_i++){
    particle_i->setOldF(particle_i->getF());
    particle_i->setF({0,0,0});
  }

  // iterate over all pairs of particles to calculate forces
  for (particle_i = particles.begin(); particle_i != --particles.end(); particle_i++) {
    auto m_i = particle_i->getM();
    auto cur_x_i = particle_i->getX();

    std::array<double, 3> cur_F_i_dummy = particle_i->getF();

    // inner loop to calculate force between particle i and all particles j after i respectfully
    for (particle_j = std::next(particle_i); particle_j!=particles.end(); particle_j++) {
      auto m_j = particle_j->getM();
      auto cur_x_j = particle_j->getX();

      std::array<double, 3> cur_F_j_dummy = particle_j->getF();

      // calculating the cubed Euclidean distance between particle i and particle j
      double norm = ArrayUtils::L2Norm(cur_x_i - cur_x_j);
      auto dividend = m_i * m_j/pow(norm, 3);

      // calculating the force components (along the x, y, z axes) between particle i and particle j
      for(int k = 0; k<3; k++){
        double force = dividend * (cur_x_j[k] - cur_x_i[k]);
        cur_F_i_dummy[k] += force;
        cur_F_j_dummy[k] -= force;
      }
      // update the force for particle i and particle j
      particle_i->setF(cur_F_i_dummy);
      particle_j->setF(cur_F_j_dummy);
    }
  }
}
