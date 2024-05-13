#include "Lenard_Jones_Force.h"

void calculateF(ParticleContainer particles) {
  std::vector<Particle>::iterator iterator_i; ///< Iterator for iterating over particles.
  std::vector<Particle>::iterator iterator_j;///< Second iterator for nested loop over particles.
  double epsilon;
  double sigma;
  // reset the force for each particle and store the old force
  for (iterator_i = particles.beginParticles(); iterator_i != particles.endParticles(); iterator_i++){
    (*iterator_i).setOldF((*iterator_i).getF());
    (*iterator_i).setF({0,0,0});
  }

  // iterate over all pairs of particles to calculate forces
  for (iterator_i = particles.beginParticles(); iterator_i != particles.endParticles();) {
    auto cur_x_i = (*iterator_i).getX();
    auto &cur_F_i = (*iterator_i).getF();
    std::array<double, 3> cur_F_i_dummy = {cur_F_i[0], cur_F_i[1], cur_F_i[2]};
     // inner loop to calculate force between particle i and all particles j after i respectfully
    for (iterator_j = ++iterator_i; iterator_j!=particles.endParticles(); iterator_j++) {
      auto cur_x_j = (*iterator_j).getX(); 
      auto &cur_F_j = (*iterator_j).getF();
      std::array<double, 3> cur_F_j_dummy = {cur_F_j[0], cur_F_j[1], cur_F_j[2]};
      // calculating the Euclidean distance between particle i and particle j
      auto norm = (*iterator_i).L2Norm(*iterator_j);
      // calulating everything for the force exept for the dircetion
      double directionlessForce = -24*epsilon/pow(norm,2)*(pow(sigma/norm,6)-2*pow(sigma/norm,12));
      // calculating the force components (along the x, y, z axes) between particle i and particle j
      for(int k = 0; k<3; k++){
        double force = directionlessForce*(cur_x_i[k]-cur_x_j[k]);
        cur_F_i_dummy[k] += force;
        cur_F_j_dummy[k] -= force;
      }
      // update the force for particle i and particle j
      (*iterator_i).setF(cur_F_i_dummy);
      (*iterator_j).setF(cur_F_j_dummy);
    }
  }
}