/**
 * @file MolSim.cpp
 * @brief Main file for Molecular Simulation for PSE. 
 * Contains methods for calculating the changing parameters of the particles as well as methods for plotting them.
 */
#include <iostream>
#include <chrono>

<<<<<<< HEAD

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

/**
 * @brief Logs a usage message.
 */
void logHelp();

double start_time = 0;       ///< The default start time of the simulation. 
double end_time = 1000;      ///< The default end time of the simulation. 
double delta_t = 0.014;      ///< The default time step of the simulation.

ParticleContainer* particles; ///< The container of the particles.
  bool linkedCellsFlag = false;
  std::array<bool,6> outflowFlag;
=======
#include <Simulation.h>
>>>>>>> f2f4e26ebf7387149824bb027964e026adb889a5

/**
 * @brief Main function for the Molecular Simulation (MolSim) program.
 * 
 * This function serves as the entry point for the Molecular Simulation program. 
 * The program first takes command-line arguments
 * to specify the input file name, and optionally, start time, end time, delta time step, and other parameters.
 * The function reads particle data from an input file and then loop runs until the current time reaches the specified end time. 
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
    auto start = std::chrono::high_resolution_clock::now();

<<<<<<< HEAD
  bool g_flag = false;
  bool i_flag = false;
  bool f_flag = false;
  bool t_flag = false;


  // Default value for vtk output. Every tenth iteration a output is generated.
  int vtk_iteration = 10;

  const char* const short_ops = "v:i:gt";
  const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"end", required_argument, nullptr, 'e'},
    {"start", required_argument, nullptr, 's'},
    {"log", required_argument, nullptr, 'l'},
    {"delta", required_argument, nullptr, 'd'},
    {"force", required_argument, nullptr, 'f'},
    {nullptr, no_argument, nullptr, 0}
  };
  int opt;

  // Initialize force object
  Force* force = nullptr;

  // Parsing command line arguments
  while((opt = getopt_long(argc, argsv, short_ops, long_opts, nullptr)) != -1){
    switch(opt){
      case 't':{
        spdlog::set_level(spdlog::level::off);
        t_flag = true;
        break;
      }
      case 'h':{
        logHelp();
        return EXIT_SUCCESS;
      }

      case 'v':{
        // parsing vtk iteration
        if(isUnsignedInt(optarg)){
          vtk_iteration = std::stoul(optarg);
          break;
        }else{
          spdlog::error("Invalid value for vtk iteration: {}", optarg);
          logHelp();
          return EXIT_FAILURE;
        }
      }

      case 'l':{
        //parsing logging level
        std::string tmp(optarg);
        if(tmp == std::string("OFF")){
          spdlog::set_level(spdlog::level::off);
          spdlog::info("Logging level set to OFF");
          break;
        }
        if(tmp == std::string("ERROR")){
          spdlog::set_level(spdlog::level::err);
          spdlog::info("Logging level set to ERROR");
          break;
        }
        if(tmp == std::string("WARN")){
          spdlog::set_level(spdlog::level::warn);
          spdlog::info("Logging level set to WARN");
          break;
        }
        if(tmp == std::string("INFO")){
          spdlog::set_level(spdlog::level::info);
          spdlog::info("Logging level set to INFO");
          break;
        }
        if(tmp == std::string("DEBUG")){
          spdlog::set_level(spdlog::level::debug);
          spdlog::info("Logging level set to DEBUG");
          break;
        }
        if(tmp == std::string("TRACE")){
          spdlog::set_level(spdlog::level::trace);
          spdlog::info("Logging level set to TRACE");
          break;
        }
        spdlog::error("Invalid logging level: {}", optarg);
        logHelp();
=======
    Simulation simulation;
    if (!simulation.initialize(argc, argsv)) {
>>>>>>> f2f4e26ebf7387149824bb027964e026adb889a5
        return EXIT_FAILURE;
    }

    simulation.run();

<<<<<<< HEAD
void calculateX() {
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
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
  if(linkedCellsFlag){
    ParticleContainerLinkedCell *LCContainer = dynamic_cast<ParticleContainerLinkedCell*>(particles);
  }
}

void calculateV() {
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
    auto m = p->getM(); ///< Mass of the particle.
    auto cur_v = p->getV(); ///< Current velocity of the particle.
    auto cur_F = p->getF(); ///< Current force acting on the particle.
    auto old_F = p->getOldF(); ///< Previous force acting on the particle.
    std::array<double, 3> cur_v_dummy = {0,0,0}; ///< Dummy array to store new velocity components.
    // calculating new velocity components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
=======
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (simulation.isTimingEnabled()) {
        std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
>>>>>>> f2f4e26ebf7387149824bb027964e026adb889a5
    }

    return 0;
}