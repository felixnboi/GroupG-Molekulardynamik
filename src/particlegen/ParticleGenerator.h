#pragma once

#include "../data/Particle.h"
#include "../data/Cuboid.h"
#include "../data/Disc.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../inputFileManager.h"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <string>

/**
 * @brief Namespace containing functions for generating particles.
 */
namespace ParticleGenerator{ 

/**
 * @brief Generates a cuboid of particles.
 * 
 * This function generates particles within a cuboid defined by its parameters.
 * The particles are written to a file.
 * 
 * @param cuboid The cuboid of particles to be generated.
 * @param filename Path of file to which particles are written.
 */     
void generateCuboid(const Cuboid& cuboid, const char *filename);

/**
 * @brief Generates a disc of particles.
 * 
 * This function generates particles within a disc defined by its parameters.
 * The particles are written to a file.
 * 
 * @param cuboid The disc of particles to be generated.
 * @param filename Path of file to which particles are written.
 */ 
void generateDisc(const Disc& disc, const char* filename);

/**
 * @brief Logs a usage message.
 */
void logHelp();

};