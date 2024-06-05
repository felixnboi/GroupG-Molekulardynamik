#include <iostream>
#include <list>
#include <math.h>
#include <getopt.h>
#include <string>
#include <chrono>

#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "utils/NumericalUtils.h"

#include "Forces/Gravitational_Force.h"
#include "Forces/Lennard_Jones_Force.h"
#include "inputFileManager.h"
#include "ParticleContainers/ParticleContainer.h"
#include "ParticleContainers/ParticleContainerLinkedCell.h"
#include "ParticleContainers/ParticleContainerOld.h"
#include "spdlog/spdlog.h"

class Simulation{
public:
    /**
     * @brief Constructor for Simulation
     */
    Simulation();

    ~Simulation();

    bool initialize(int argc, char* argv[]);

    void run();

    bool isTimingEnabled() const;

    void cleanup();

private:
    double start_time;
    double end_time;
    double delta_t;
    int vtk_iteration;
    bool timing_enabled;

    std::unique_ptr<ParticleContainer> particles;
    std::unique_ptr<Force> force;
    std::string input_file;
    std::string input_file_user;
    bool g_flag;
    bool i_flag;
    bool f_flag;
    bool t_flag;
    std::array<bool,6> lenJonesBouneryFlags;
    std::array<bool,6> outflowFlags;
    bool linkedCellsFlag;
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