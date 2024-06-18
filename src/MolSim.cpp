/**
 * @file MolSim.cpp
 * @brief Main file for Molecular Simulation for PSE. 
 * Contains methods for calculating the changing parameters of the particles as well as methods for plotting them.
 */
#include <iostream>
#include <chrono>

#include <Simulation.h>

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

    Simulation simulation;
    if (!simulation.initialize(argc, argsv)) {
        return EXIT_FAILURE;
    }

    simulation.run();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (simulation.isTimingEnabled()) {
        std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    }

    return 0;
}