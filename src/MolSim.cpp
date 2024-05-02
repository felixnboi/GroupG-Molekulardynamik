/**
 * @file MolSim.cpp
 * @brief Main file for Molecular Simulation for PSE. 
 * Contains methods for calculating the changing parameters of the particles as well as methods for plotting them.
 */

#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <math.h>

/**
 * @brief Calculate the euclidean norm of the distance between two particles.
 * 
 * @param particle1 First particle.
 * @param particle2 Second particle.
 * @return The euclidean norm of the distance between the two particles.
 */
double euclidean_norm_x(const Particle &particle1, const Particle &particle2);

/**
 * @brief Calculate the force for all particles.
 * 
 * This function calculates 
 */
void calculateF();

/**
 * @brief Calculate the position for all particles.
 */
void calculateX();

/**
 * @brief Calculate the velocity for all particles.
 */
void calculateV();

/**
 * @brief Plot the particles to a vtu-file.
 * 
 * @param iteration The current iteration number.
 */
void plotParticles(int iteration);

constexpr double start_time = 0; ///< The start time of the simulation. 
double end_time = 1000; ///< The end time of the simulation. 
double delta_t = 0.014; ///< The time step of the simulation.

std::list<Particle> particles; ///< The list of particles.

/**
 * @brief Main function for the Molecular Simulation (MolSim) program.
 * 
 * This function serves as the entry point for the Molecular Simulation program. 
 * The program first takes command-line arguments
 * to specify the input file name, end time, and time step. The function reads
 * particle data from an input file, performs a simulation over a specified time period 
 * with a given timestep duration, and writes the simulation results to output files. 
 * Iterating through the simulation time steps, the function updates the positions and velocities of particles based on calculated forces,
 * and periodically () writes the particle positions to VTK files for visualization.
 * 
 * @param argc The number of command-line arguments.
 * @param argsv An array of pointers to the command-line arguments.
 * @return The exit status of the program.
 * 
 * @param argc Number of command-line arguments.
 * @param argsv Array of command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 4) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "Usage: ./molsym filename end_time delta_t" << std::endl;
  }

  FileReader fileReader;
  fileReader.readFile(particles, argsv[1]);
  if(particles.size() <= 0){
    std::cout << "No particles!" << std::endl;
    return EXIT_FAILURE;
  }

  end_time = std::atof(argsv[2]);
  delta_t = std::atof(argsv[3]);

  std::cout << "end_time:" << end_time << ", delta_t:" << delta_t << "\n";

  double current_time = start_time;

  int iteration = 0;
  plotParticles(iteration);

  while (current_time < end_time) {
    calculateX();
    calculateF();
    calculateV();

    iteration++;
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void calculateF() {
  std::list<Particle>::iterator iterator_i;
  iterator_i = particles.begin();
  std::list<Particle>::iterator iterator_j;

  for(auto &p : particles){
    p.setOldF(p.getF());
    p.setF({0,0,0});
  }
  for (size_t i = 0; i<particles.size()-1; i++) {
    auto &cur_particle_i = *(iterator_i++);
    auto m_i = cur_particle_i.getM();
    auto cur_x_i = cur_particle_i.getX();
    auto &cur_F_i = cur_particle_i.getF();
    std::array<double, 3> cur_F_i_dummy = {cur_F_i[0], cur_F_i[1], cur_F_i[2]};
    for (iterator_j = iterator_i; iterator_j!=particles.end(); iterator_j++) {
      auto &cur_particle_j = *iterator_j;
      auto m_j = cur_particle_j.getM();
      auto cur_x_j = cur_particle_j.getX(); 
      auto &cur_F_j = cur_particle_j.getF();
      std::array<double, 3> cur_F_j_dummy = {cur_F_j[0], cur_F_j[1], cur_F_j[2]};

      auto norm = euclidean_norm_x(cur_particle_i, cur_particle_j);
      for(int k = 0; k<3; k++){
        double tmp = m_i * m_j / pow(norm, 3) * (cur_x_j[k] - cur_x_i[k]);
        cur_F_i_dummy[k] += tmp;
        cur_F_j_dummy[k] -= tmp;
      }
      cur_particle_i.setF(cur_F_i_dummy);
      cur_particle_j.setF(cur_F_j_dummy);
    }
  }
}

double euclidean_norm_x(const Particle &particle1, const Particle &particle2){
  double sum = 0.0;
  for (int i = 0; i<3; i++){
    sum += pow(particle1.getX().at(i) - particle2.getX().at(i), 2);
  }
  return pow(sum, 0.5);
}

void calculateX() {
  for (auto &p : particles) {
    auto m = p.getM();
    auto cur_x = p.getX();
    auto cur_v = p.getV();
    auto cur_F = p.getF();
    std::array<double, 3> cur_x_dummy = {0,0,0};
    for(int i = 0; i<3; i++){
      cur_x_dummy[i] = cur_x[i] + delta_t * cur_v[i] + delta_t * delta_t * cur_F[i] / (2*m); 
    }
    p.setX(cur_x_dummy);
  }
}

void calculateV() {
  for (auto &p : particles) {
    auto m = p.getM();
    auto cur_v = p.getV();
    auto cur_F = p.getF();
    auto old_F = p.getOldF();
    std::array<double, 3> cur_v_dummy = {0,0,0};
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
    }
    p.setV(cur_v_dummy);
  }
}

/**
 * @brief Plot the particles to a VTK file.
 * 
 * This function uses the outputwriter of a VTK type to plot the particles into a file 
 * via the methods of VTKWriter class. Firstly, 
 * 
 * 
 * @param iteration The current iteration number.
 */
void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::VTKWriter writer; ///< The VTK writer. 
  writer.initializeOutput(particles.size());
  for(auto &p : particles){
    writer.plotParticle(p);
  }
  writer.writeFile(out_name, iteration);
}
