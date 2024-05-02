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
 * @brief Calculate the Euclidean Norm of the distance between two particles.
 * 
 * This function computes the Euclidean distance between two particles
 * in three-dimensional space. It takes two Particle objects as input
 * and calculates the Euclidean distance between their positions. The
 * Euclidean distance is computed as the square root of the sum of the
 * squares of the differences in the coordinates of the particles along
 * each dimension (x, y, and z).
 * 
 * @param particle1 First particle.
 * @param particle2 Second particle.
 * @return The Euclidean Norm of the distance between the two particles.
 */
double euclidean_norm_x_cubed(const Particle &particle1, const Particle &particle2);

/**
 *test if this string is a double 
 */ 
bool testIfStringIsDouble(char * string);

/**
 * @brief Calculate the force for all particles.
 * 
 * This function calculates the force acting on each particle due to gravitational
 * interactions with other particles. Firstly, it updates the old_f parameter (represents the previous force 
 * value for the current timestep) of each particle and sets the current Force parameters (f) to 0. 
 * After that, the function iterates over all pairs of particles and
 * computes the gravitational force between them based on their masses and positions.
 * The calculated forces are then updated (added to the current Forces of according particles) for each particle.
 * 
 * @note This function assumes that the old force for each particle has already been
 * stored using the setOldF method before calling this function.
 * @see euclidean_norm_x() To calculate the Euclidean distance between particles.
 */
void calculateF();

/**
 * @brief Calculate the position for all particles.
 * 
 * This function calculates and updates the new position of each particle based on its current position, 
 * velocity vector, current forces vector and mass(the last two are used for calculating the acceleration). 
 * It employs the Velocity-Störmer-Verlet method to advance the particle positions in time. 
 */
void calculateX();

/**
 * @brief Calculate the velocity for all particles.
 * 
 * This function updates the velocity of each particle via the Velocity-Störmer-Verlet method
 * for the current time moment:
 * The new (current) velocity is calculated using particle´s previous velocity, the forces that were acting 
 * on the particle in the previous time stamp, the forces that are acting on it currently, 
 * and the particle´s mass.
 * 
 * @note This function assumes that the old force for each particle has already been
 * stored using the setOldF method before calling this function.
 */
void calculateV();

/**
 * @brief Plot the particles to a VTK file.
 * 
 * This function uses the outputwriter of a VTK type to plot the particles into a file 
 * via the methods of VTKWriter class. As stated in VTKWriter.h notes, this function first prepares to plot a particle by 
 * initializing internal data structures, then plots it and writes the output of the iteration to a unique file
 * with a given filename base.
 * 
 * @param iteration The current iteration number.
 */
void plotParticles(int iteration);

double start_time; ///< The start time of the simulation. 
double end_time; ///< The end time of the simulation. 
double delta_t; ///< The time step of the simulation.

std::list<Particle> particles; ///< The list of particles.

/**
 * @brief Main function for the Molecular Simulation (MolSim) program.
 * 
 * This function serves as the entry point for the Molecular Simulation program. 
 * The program first takes command-line arguments
 * to specify the input file name, and optionally, start time, end time, and delta time step. The function reads
 * particle data from an input file and then loop runs until the current time reaches the specified end time. 
 * In each iteration, the position, velocity, and forces of the particles are updated using the calculateX, 
 * calculateF, and calculateV functions, respectively. The particle positions are periodically 
 * (at intervals of 10 iterations) plotted using the plotParticles function.
 * Finally, an output message is displayed, indicating that the simulation is complete, and the program
 * terminates with a success status.
 * 
 * @param argc The number of command-line arguments.
 * @param argsv An array of pointers to the command-line arguments.
 * @return The exit status of the program.
 * @see calculateX() To calculate the position for all particles.
 * @see calculateF() To calculate the force for all particles.
 * @see calculateV() To calculate the velocity for all particles.
 * @see plotParticles() To plot the particles to a VTK file.
 */
int main(int argc, char *argsv[]) {
  std::cout << "Hello from MolSim for PSE!" << std::endl;

  if(argc==2){
    std::cout << "default values end_time = 1000 delta_t = 0.014 start_time = 0 are used" << std::endl;
    start_time = 0;
    end_time = 1000;
    delta_t = 0.014;
  }else{
    if(argc == 4) {
    if(!(testIfStringIsDouble(argsv[2])&&testIfStringIsDouble(argsv[3]))) {
      std::cout << "arguments for end_time or delta_t are not numerical values" << std::endl;
      std::cout << "Usage: ./MolSim <path/to/input/file> [[end_time delta_t] |[end_time delta_t start_time]" << std::endl;
      return EXIT_FAILURE;
    }
      std::cout << "custom values for end_time delta_t are used. start_time = 0" << std::endl;
      start_time = 0;
      end_time = std::atof(argsv[2]);
      delta_t = std::atof(argsv[3]);
    }else{
      if(argc == 5){
        if(!(testIfStringIsDouble(argsv[2])&&testIfStringIsDouble(argsv[3])&&testIfStringIsDouble(argsv[4]))) {
          std::cout << "arguments for end_time, delta_t or start_time are not numerical values" << std::endl;
          std::cout << "Usage: ./MolSim <path/to/input/file> [[end_time delta_t] |[end_time delta_t start_time]" << std::endl;
          return EXIT_FAILURE;
        }
        std::cout << "custom values for end_time, delta_t and start_time are used" << std::endl;
        end_time = std::atof(argsv[2]);
        delta_t = std::atof(argsv[3]);
        start_time = std::atof(argsv[4]);
      }else{
        std::cout << "errounous program call" << std::endl;
        std::cout << "Usage: ./MolSim <path/to/input/file> [[end_time delta_t] |[end_time delta_t start_time]" << std::endl;
      }
    }
  }

  if(start_time > end_time){
    std::cout << "Error: start_time is after end_time" << std::endl;
    return EXIT_FAILURE;
  }
  
  FileReader fileReader;
  fileReader.readFile(particles, argsv[1]);

  // checking if there are particles in the simulation
  if(particles.size() <= 0){
    std::cout << "Failed to read Particles from input file!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "end_time:" << end_time << ", delta_t:" << delta_t << ", start_time:" << start_time << std::endl;

  double current_time = 0;
  int iteration = 0;

  while (current_time < start_time){
    calculateX();
    calculateF();
    calculateV();
    current_time += delta_t;
    iteration++;
  }

  //simulation loop
  while (current_time < end_time) {
    calculateX();
    calculateF();
    calculateV();
    iteration++;

    // plotting particle positions only at intervals of 10 iterations
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    // printing simulation progress
    std::cout << "Iteration " << iteration << " finished." << std::endl;
    // update simulation time
    current_time += delta_t;
  }
  // display output message and terminate the program
  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

bool testIfStringIsDouble(char * string){
  while (isdigit(*string)){
    string++;   
  }   
  if(*string == '.') {    
    string++;   
  }   
  while (isdigit(*string)){    
    string++;   
  }
    return *string == '\0'; 
}


void calculateF() {
  std::list<Particle>::iterator iterator_i; ///< Iterator for iterating over particles.
  iterator_i = particles.begin();
  std::list<Particle>::iterator iterator_j;///< Second iterator for nested loop over particles.

  // reset the force for each particle and store the old force
  for(auto &p : particles){
    p.setOldF(p.getF());
    p.setF({0,0,0});
  }

  // iterate over all pairs of particles to calculate forces
  for (size_t i = 0; i<particles.size()-1; i++) {
    auto &cur_particle_i = *(iterator_i++);
    auto m_i = cur_particle_i.getM();
    auto cur_x_i = cur_particle_i.getX();
    auto &cur_F_i = cur_particle_i.getF();
    std::array<double, 3> cur_F_i_dummy = {cur_F_i[0], cur_F_i[1], cur_F_i[2]};
     // inner loop to calculate force between particle i and all particles j after i respectfully
    for (iterator_j = iterator_i; iterator_j!=particles.end(); iterator_j++) {
      auto &cur_particle_j = *iterator_j;
      auto m_j = cur_particle_j.getM();
      auto cur_x_j = cur_particle_j.getX(); 
      auto &cur_F_j = cur_particle_j.getF();
      std::array<double, 3> cur_F_j_dummy = {cur_F_j[0], cur_F_j[1], cur_F_j[2]};
      // calculating the cubed Euclidean distance between particle i and particle j
      auto dividend = m_i * m_j/euclidean_norm_x_cubed(cur_particle_i, cur_particle_j);
      // calculating the force components (along the x, y, z axes) between particle i and particle j
      for(int k = 0; k<3; k++){
        double force = dividend * (cur_x_j[k] - cur_x_i[k]);
        cur_F_i_dummy[k] += force;
        cur_F_j_dummy[k] -= force;
      }
      // update the force for particle i and particle j
      cur_particle_i.setF(cur_F_i_dummy);
      cur_particle_j.setF(cur_F_j_dummy);
    }
  }
}

double euclidean_norm_x_cubed(const Particle &particle1, const Particle &particle2){
  double sum = 0.0; ///< A variable that is used for the sum of squared differences.
  // iterate over the x, y, and z coordinates of the particles
  for (int i = 0; i<3; i++){
    // add the squared difference along each dimension to the sum
    sum += pow(particle1.getX().at(i) - particle2.getX().at(i), 2);
  }
  // return the cubed square root of the sum to obtain the cubed Euclidean distance
  return pow(sum, 1.5);
}

void calculateX() {
  // iterating over all particles to calculate new positions
  for (auto &p : particles) {
    auto m = p.getM(); ///< Mass of the particle.
    auto cur_x = p.getX(); ///< Current position of the particle.
    auto cur_v = p.getV(); ///< Current velocity of the particle.
    auto cur_F = p.getF(); ///< Current force acting on the particle.
    std::array<double, 3> cur_x_dummy = {0,0,0}; ///< Dummy array to store new position components.

    // calculating new position components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_x_dummy[i] = cur_x[i] + delta_t * cur_v[i] + delta_t * delta_t * cur_F[i] / (2*m); 
    }
    // set the new position for the particle
    p.setX(cur_x_dummy);
  }
}

void calculateV() {
  // iterating over all particles to calculate new positions
  for (auto &p : particles) {
    auto m = p.getM(); ///< Mass of the particle.
    auto cur_v = p.getV(); ///< Current velocity of the particle.
    auto cur_F = p.getF(); ///< Current force acting on the particle.
    auto old_F = p.getOldF(); ///< Previous force acting on the particle.
    std::array<double, 3> cur_v_dummy = {0,0,0}; ///< Dummy array to store new velocity components.
    // calculating new velocity components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
    }
    // set the new velocity for the particle
    p.setV(cur_v_dummy);
  }
}


void plotParticles(int iteration) {

  std::string out_name("MD_vtk"); ///< The base name of the VTK file to be written.

  outputWriter::VTKWriter writer; ///< The VTK writer object. 
  // initializing the VTK writer with the total number of particles.
  writer.initializeOutput(particles.size()); 
  // iterating over each particle to plot its position
  for(auto &p : particles){
    writer.plotParticle(p);
  }
  // write the plotted particle positions to a VTK file
  writer.writeFile(out_name, iteration);
}
