/**
 * @file MolSim.cpp
 * @brief Main file for Molecular Simulation for PSE. 
 * Contains methods for calculating the changing parameters of the particles as well as methods for plotting them.
 */

#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "Forces/GravitationalForce.h"
#include "Forces/Lenard_Jones_Force.h"
#include "ParticleGenerator.h"

#include <iostream>
#include <list>
#include <math.h>
#include <unistd.h>
#include "spdlog/spdlog.h"

/**
 *test if this string is a double 
 */ 
bool isDouble(char *string);

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
//void calculateF();

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

double start_time = 0;       ///< The default start time of the simulation. 
double end_time = 1000;      ///< The default end time of the simulation. 
double delta_t = 0.014;      ///< The default time step of the simulation.

ParticleContainer particles; ///< The container of the particles.

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
  int opt;
  std::string input_file = "../input/eingabe-sonne.txt";

  spdlog::set_level(spdlog::level::trace);
  spdlog::info("INFO");
  spdlog::trace("TRACE");

  while((opt = getopt(argc, argsv, "d:e:s:f:")) != -1){
    switch(opt){

      case 'd':
        if(isDouble(optarg)){
          delta_t = atof(optarg);
          break;
        }else{
          std::cout << "error\n";
          return EXIT_FAILURE;
        }

      case 'e':
        if(isDouble(optarg)){
          end_time = atof(optarg);
          break;
        }else{
          std::cout << "error\n";
          return EXIT_FAILURE;
        }

      case 's':
        if(isDouble(optarg)){
          start_time = atof(optarg);
          break;
        }else{
          std::cout << "error\n";
          return EXIT_FAILURE;
        }
      case 'f':
        input_file = optarg;
        break;
      case '?':
        std::cout << "error\n";
        return EXIT_FAILURE;
    }
  }

  Force* force = new GravitationalForce();

  if(start_time > end_time){
    std::cout << "Error: start_time is after end_time" << std::endl;
    return EXIT_FAILURE;
  }
  
  FileReader fileReader;
  fileReader.readFile(particles, input_file.c_str());

  // checking if there are particles in the simulation
  if(particles.getParticles().empty()){
    std::cout << "Failed to read Particles from input file!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "end_time:" << end_time << ", delta_t:" << delta_t << ", start_time:" << start_time << std::endl;

  double current_time = 0;
  int iteration = 0;

  while (current_time < start_time){
    calculateX();
    force->calculateF(particles);
    calculateV();
    current_time += delta_t;
    iteration++;
  }

  //simulation loop
  while (current_time < end_time) {
    calculateX();
    force->calculateF(particles);
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

bool isDouble(char *string){
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

void calculateX() {
  // iterating over all particles to calculate new positions
  for (auto p = particles.beginParticles(); p != particles.endParticles(); p++){
    auto m = p->getM(); ///< Mass of the particle.
    auto cur_x = p->getX(); ///< Current position of the particle.
    auto cur_v = p->getV(); ///< Current velocity of the particle.
    auto cur_F = p->getF(); ///< Current force acting on the particle.
    std::array<double, 3> cur_x_dummy = {0,0,0}; ///< Dummy array to store new position components.

    // calculating new position components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_x_dummy[i] = cur_x[i] + delta_t * cur_v[i] + delta_t * delta_t * cur_F[i] / (2*m); 
    }
    // set the new position for the particle
    p->setX(cur_x_dummy);
  }
}

void calculateV() {
  // iterating over all particles to calculate new positions
  for (auto p = particles.beginParticles(); p != particles.endParticles(); p++){
    auto m = p->getM(); ///< Mass of the particle.
    auto cur_v = p->getV(); ///< Current velocity of the particle.
    auto cur_F = p->getF(); ///< Current force acting on the particle.
    auto old_F = p->getOldF(); ///< Previous force acting on the particle.
    std::array<double, 3> cur_v_dummy = {0,0,0}; ///< Dummy array to store new velocity components.
    // calculating new velocity components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
    }
    // set the new velocity for the particle
    p->setV(cur_v_dummy);
  }
}


void plotParticles(int iteration) {

  std::string out_name("MD_vtk"); ///< The base name of the VTK file to be written.

  outputWriter::VTKWriter writer; ///< The VTK writer object. 
  // initializing the VTK writer with the total number of particles.
  writer.initializeOutput(particles.getParticles().size()); 
  // iterating over each particle to plot its position
  for(auto p : particles.getParticles()){
    writer.plotParticle(p);
  }
  // write the plotted particle positions to a VTK file
  writer.writeFile(out_name, iteration);
}