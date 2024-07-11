#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

#include <fstream>
/**
 * @brief Namespace containing functions for writing checkpoints.
 */
namespace CheckpointWriter{
/**
 * @brief Writes checkpoint data to a given file.
 * 
 * @param particles The container of particles to be considered.
 * @param filename Path of file to which data is written.
 */     
    void writeCheckpoint(ParticleContainer &particles,  const char *filename);
}