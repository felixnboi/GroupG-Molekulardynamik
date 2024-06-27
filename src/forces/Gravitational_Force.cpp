#include "Gravitational_Force.h"

Gravitational_Force::Gravitational_Force() {
  spdlog::info("GravitationalForce object constructed");
};
Gravitational_Force::~Gravitational_Force() {
  spdlog::info("GravitationalForce object destructed");
};


void Gravitational_Force::calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) {
  // reset the force for each particle, store the old force and claculates the GravitationalForce
  for(auto particle : particles.getParticles()){
    if (!particle->getIsOuter()) {
    particle->setOldF(particle->getF());
    std::array<double,3> gravForce = {0, particle->getM()*gravConstant,0};
    particle->setF(gravForce);
    }
  }
  std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs = particles.getParticlePairs();
  std::vector<std::shared_ptr<Particle>>::iterator particle_i; ///< Iterator for iterating over particles.
  std::vector<std::shared_ptr<Particle>>::iterator particle_j; ///< Second iterator for nested loop over particles.

  // iterate over all pairs of particles to calculate forces
  for (auto pair = particlePairs.begin(); pair != particlePairs.end(); pair++){
    *particle_i = (*pair)[0];
    *particle_j = (*pair)[1];

    auto m_i = (*particle_i)->getM();
    auto cur_x_i = (*particle_i)->getX();

    std::array<double, 3> cur_F_i_dummy = (*particle_i)->getF();

  // inner loop to calculate force between particle i and all particles j after i respectfully
    auto m_j = (*particle_j)->getM();
    auto cur_x_j = (*particle_j)->getX();

    std::array<double, 3> cur_F_j_dummy = (*particle_j)->getF();

    // calculating the cubed Euclidean distance between particle i and particle j
    double norm = ArrayUtils::L2Norm(cur_x_i - cur_x_j);
    auto dividend = m_i * m_j/pow(norm, 3);

    // calculating the force components (along the x, y, z axes) between particle i and particle j
    for(int k = 0; k<3; k++){
      double force = dividend * (cur_x_j[k] - cur_x_i[k]);
      if (!(*particle_i)->getIsOuter()) {cur_F_i_dummy[k] += force;}
      if (!(*particle_j)->getIsOuter()) {cur_F_j_dummy[k] -= force;}
    }
    // update the force for particle i and particle j
    if (!(*particle_i)->getIsOuter()) {(*particle_i)->setF(cur_F_i_dummy);}
    if (!(*particle_j)->getIsOuter()) {(*particle_j)->setF(cur_F_j_dummy);}
  }
}
