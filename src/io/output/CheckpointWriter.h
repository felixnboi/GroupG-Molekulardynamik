#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

#include <fstream>

namespace CheckpointWriter
{
    void writeCheckpoint(ParticleContainer &particles,  const char *filename);
}