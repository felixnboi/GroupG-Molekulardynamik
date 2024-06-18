#include <iostream>
#include <list>
#include <math.h>
#include <getopt.h>
#include <string>
#include <chrono>
#include <array>

#include "io/input/FileReader.h"
#include "io/input/XMLReader.h"
#include "io/output/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "utils/NumericalUtils.h"

#include "Forces/Gravitational_Force.h"
#include "Forces/Lennard_Jones_Force.h"
#include "inputFileManager.h"
#include "ParticleContainers/ParticleContainer.h"
#include "ParticleContainers/ParticleContainerLinkedCell.h"
#include "ParticleContainers/ParticleContainerOld.h"
#include "spdlog/spdlog.h"
#include "io/input/XMLReader.h"
#include "io/input/XMLFormat.h"
#include "data/SimData.h"
#include "data/Thermostat.h"

/**
 * @class Simulation
 * @brief Manages the simulation lifecycle including initialization, execution, and cleanup.
 * 
 * The Simulation class encapsulates all the necessary components and processes for 
 * running a particle simulation. It handles the initialization of parameters, 
 * execution of the simulation loop, timing control, and cleanup after the simulation ends.
 */
class Simulation{
public:

    /**
     * @brief Constructor for Simulation.
     * 
     * Initializes a new instance of the Simulation class with default settings.
     */
    Simulation();

    /**
     * @brief Destructor for Simulation.
     * 
     * Cleans up resources used by the Simulation instance.
     */
    ~Simulation();

    /**
     * @brief Initializes the simulation with command-line arguments.
     * 
     * This method processes command-line arguments to set up simulation parameters 
     * such as input file, timing flags, and other options.
     * 
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line argument strings.
     * @return True if initialization is successful, otherwise false.
     */
    bool initialize(int argc, char* argv[]);

    /**
     * @brief Runs the simulation.
     * 
     * Executes the main simulation loop, updating particle positions and velocities 
     * over time and plotting the particle states at each iteration.
     */
    void run();
    
    /**
     * @brief Checks whether timing should be enabled for the simulation.
     * 
     * @return True if timing is enabled, otherwise false.
     */
    bool isTimingEnabled() const;

private:
    SimData simdata;
    Thermostat thermostat;

    std::unique_ptr<ParticleContainer> particles;
    std::unique_ptr<Force> force;

    bool xml_flag;
    bool generate_flag;
    bool input_flag;
    bool force_flag;
    bool time_flag;
    bool cli_flag;
    bool linkedcell_flag;
    bool thermostat_flag;

    std::array<bool,6> lenJonesBoundaryFlags;
    std::array<bool,6> outflowFlags;

    std::string input_file_user;


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

};