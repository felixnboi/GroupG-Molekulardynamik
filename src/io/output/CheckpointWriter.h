#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

#include <fstream>

/**
 * @brief The Checkpointwriter namespace wich implements a function to write the current state of the simulation to a file.
 */
namespace CheckpointWriter{

    /**
     * @brief This function writes the current state of the simulation to a file. 
     * 
     * @param particles The particle container that represents the simulation.
     * @param filename The name of the file to wich the state of the simualtion is written to.
     */
    void writeCheckpoint(ParticleContainer &particles,  const char *filename);
}