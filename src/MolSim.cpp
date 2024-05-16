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
#include "inputFileManager.h"

#include <iostream>
#include <list>
#include <math.h>
#include <unistd.h>
#include "spdlog/spdlog.h"

/**
 *@brief Test if the given string is a double 
 */ 
bool isDouble(char *string);

/**
 *@brief Test if this string is an unsigned int 
 */ 
bool isUnsignedInt(char* str);

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
 * In each iteration, the position, velocity, and forces of the particles are updated respectively. The particle positions are periodically 
 * (usually, at intervals of 10 iterations) plotted using the plotParticles function.
 * Finally, an output message is displayed, indicating that the simulation is complete, and the program
 * terminates with a success status.
 * 
 * @param argc The number of command-line arguments.
 * @param argsv An array of pointers to the command-line arguments.
 * @return The exit status of the program.
 * @see calculateX() To calculate the position for all particles.
 * @see calculateF() To calculate the force for all particles.
 * @see plotParticles() To plot the particles to a VTK file.
 */
int main(int argc, char *argsv[]) {

  spdlog::set_level(spdlog::level::trace);
  spdlog::info("Hello from MolSim for PSE!");
  int opt;
  std::string input_file;

  bool g_flag = false;
  bool i_flag = false;

  int vtk_iteration = 10;

  // Initialize force object
  Force* force = new Lenard_Jones_Force();

  // Parsing command line arguments
  while((opt = getopt(argc, argsv, "v:l:d:e:s:i:f:g")) != -1){
    switch(opt){
      case 'v':{
        // parsing vtk iteration
        if(isUnsignedInt(optarg)){
          vtk_iteration = std::stoul(optarg);
          break;
        }else{
          spdlog::error("Invalid value for vtk iteration: {}", optarg);
          return EXIT_FAILURE;
        }
      }

      case 'l':{
        //parsing logging level
        std::string tmp{optarg};
        if(tmp == "OFF"){
          spdlog::set_level(spdlog::level::off);
          spdlog::info("Logging level set to OFF");
          break;
        }
        if(tmp == "ERROR"){
          spdlog::set_level(spdlog::level::err);
          spdlog::info("Logging level set to ERROR");
          break;
        }
        if(tmp == "WARN"){
          spdlog::set_level(spdlog::level::warn);
          spdlog::info("Logging level set to WARN");
          break;
        }
        if(tmp == "INFO"){
          spdlog::set_level(spdlog::level::info);
          spdlog::info("Logging level set to INFO");
          break;
        }
        if(tmp == "DEBUG"){
          spdlog::set_level(spdlog::level::debug);
          spdlog::info("Logging level set to DEBUG");
          break;
        }
        if(tmp == "TRACE"){
          spdlog::set_level(spdlog::level::trace);
          spdlog::info("Logging level set to TRACE");
          break;
        }
        spdlog::error("Invalid logging level: {}", optarg);
        return EXIT_FAILURE;
      }

      case 'g':{
        g_flag = true;
        break;
      }

      case 'd':{
        if(isDouble(optarg)){
          delta_t = atof(optarg);
          break;
        }else{
          spdlog::error("Invalid argument for delta_t");
          return EXIT_FAILURE;
        }
      }

      case 'e':{
        if(isDouble(optarg)){
          end_time = atof(optarg);
          break;
        }else{
          spdlog::error("Invalid argument for end_time");
          return EXIT_FAILURE;
        }
      }

      case 's':{
        if(isDouble(optarg)){
          start_time = atof(optarg);
          break;
        }else{
          spdlog::error("Invalid argument for start_time");
          return EXIT_FAILURE;
        }
      }
      
      case 'i':{
        i_flag = true;
        input_file = optarg;
        break;
      }

      case 'f':{
        if(*optarg == 'g'){
          delete force;
          force = new GravitationalForce();
          spdlog::info("Force set to Gravitational_Force");
          break;
        }
        if(*optarg == 'l'){
          spdlog::info("Force set to Lenard_Jones_Force");
          break;
        }
        spdlog::error("Invalid argument for force");
        return EXIT_FAILURE;
      }

      case '?':{
        spdlog::error("Invalid option");
        return EXIT_FAILURE;
      }
    }
  }

  if(!g_flag){
    inputFileManager::resetFile();
    spdlog::info("Generated input file reset");
  }

  if(i_flag){
    inputFileManager::mergeFile(input_file.c_str());
    spdlog::info("File {} merged into generated input file", input_file);
  }

  input_file = "../input/generated-input.txt";

  // Check if start_time is after end_time
  if(start_time > end_time){
    spdlog::error("Error: start_time should not be after end_time!");
    return EXIT_FAILURE;
  }
  
  FileReader fileReader;
  fileReader.readFile(particles, input_file.c_str());

  // checking if there are particles in the simulation
  if(particles.getParticles().empty()){
    spdlog::error("Failed to read Particles from input file!");
    return EXIT_FAILURE;
  }

  spdlog::info("end_time:{}, delta_t:{}, start_time:{}", end_time, delta_t, start_time);

  double current_time = 0;
  int iteration = 0;

  // Advance simulation time to start_time
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

    // plotting particle positions only at intervals of iterations
    if (iteration % vtk_iteration == 0) {
      plotParticles(iteration);
    }
    // printing simulation progress
    spdlog::info("Iteration {} finished", iteration);
    // update simulation time
    current_time += delta_t;
  }
  // display output message and terminate the program
  spdlog::info("Output written. Terminating...");

  delete force;
  return 0;
}


bool isDouble(char* str) {
    try {
        std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool isUnsignedInt(char* str) {
    try {
        unsigned long value = std::stoul(str);
        return value <= std::numeric_limits<unsigned int>::max();
    } catch (...) {
        return false;
    }
}


void calculateX() {
  // iterating over all particles to calculate new positions
  for (auto p = particles.begin(); p != particles.end(); p++){
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
  for (auto p = particles.begin(); p != particles.end(); p++){
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
  for(const auto& p : particles.getParticles()){
    writer.plotParticle(p);
  }
  // write the plotted particle positions to a VTK file
  writer.writeFile(out_name, iteration);
}