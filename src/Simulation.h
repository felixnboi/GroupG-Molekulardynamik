#include <iostream>
#include <list>
#include <math.h>
#include <getopt.h>
#include <string>
#include <chrono>
#include <array>
#include <omp.h>

#include "io/input/FileReader.h"
#include "io/input/XMLReader.h"
#include "io/output/VTKWriter.h"
#include "io/output/CheckpointWriter.h"
#include "io/output/ProfilingComponent.h"
#include "utils/ArrayUtils.h"
#include "utils/NumericalUtils.h"

#include "forces/Force.h"
#include "inputFileManager.h"
#include "particlecontainers/ParticleContainer.h"
#include "particlecontainers/ParticleContainerLinkedCell.h"
#include "particlecontainers/ParticleContainerOld.h"
#include "spdlog/spdlog.h"
#include "io/input/XMLReader.h"
#include "io/input/XMLFormat.h"
#include "data/SimData.h"
#include "data/ThermostatData.h"
#include "data/CheckpointData.h"
#include "data/MembraneData.h"
#include "thermostat/Thermostat.h"

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
     * 
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
    

private:
    SimData simdata; ///< Holds the simulation data.
    Thermostat thermostat; ///< Thermostat for temperature control in simulation.
    ThermostatData thermostat_data; ///< Holds thermostat data.
    CheckpointData checkpoint_data; ///< Holds checkpoint data.
    MembraneData membrane_data; ///< Holds membrane data.

    std::unique_ptr<ParticleContainer> particles; ///< Container holding simulaton particles.
    std::unique_ptr<Force> force; ///< Force object for force calculations.

    bool xml_flag{}; ///< Flag indicating whether an XML file is used for input.
    bool generate_flag{}; ///< Flag indicating whether particle generation is enabled.
    bool input_flag{}; ///< Flag indicating whether a user input file is specified.
    bool force_flag{}; ///< Flag indicating whether a force type is specified.
    bool time_flag{}; ///< Flag indicating whether timing information should be logged.
    bool cli_flag{}; ///< Flag indicating whether command-line interface (CLI) options are used.
    bool linkedcell_flag{}; ///< Flag indicating whether the linked cell algorithm is used.
    bool lenJones_flag{}; ///< Flag indicating whether the lennard Jones Force should be used for force calculation.
    bool grav_flag{};  ///< Flag indicating whether the gravitational Force should be used for force calculation.

    //The order for two following Flags: {left, right, bottom, top, back, front}.
    std::array<bool,6> lenJonesBoundaryFlags{}; ///< Flags for Lennard-Jones boundary conditions.
    std::array<bool,6> outflowFlags{}; ///< Flags for outflow boundary conditions.
    //The order for the following flag: {left-right, bottom-top, back-front}.
    std::array<bool,3> periodicFlags{}; ///< Flags for periodic boundary conditions. 

    std::string input_file_user; ///< User-specified input file name for loading initial particle configurations.


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