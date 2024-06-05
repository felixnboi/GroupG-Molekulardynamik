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
 * The particles are written to an input file.
 * 
 * @param cuboid The cuboid of particles to be generated.
 * @param filename Path of file to which particles are written.
 */     
void generateCuboid(const Cuboid& cuboid, const char *filename);

/**
 * Generates a disc of particles and writes to the specified file.
 *
 * @param x x-coordinate of the center of the disc.
 * @param y y-coordinate of the center of the disc.
 * @param z z-coordinate of the center of the disc.
 * @param MoleculesPerRadius Number of molecules along the radius.
 * @param distance Distance (mesh width) between the molecules.
 * @param mass Mass of each molecule.
 * @param velocityX Initial velocity along the x-axis for the whole disc.
 * @param velocityY Initial velocity along the y-axis for the whole disc.
 * @param velocityZ Initial velocity along the z-axis for the whole disc.
 * @param filename File to which the particles will be written.
 */
void generateDisc(const Disc& disc, const char* filename);

/**
 * @brief Logs a usage message.
 */
void logHelp();
};