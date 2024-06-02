#pragma once

#include "../Particle.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../inputFileManager.h"
#include "spdlog/spdlog.h"

#include <cstdlib>
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
 * @brief Generates a cuboid of particles with specified parameters.
 * 
 * This function generates particles within a cuboid that is defined by the parameters.
 * The particles are written to an input file.
 * 
 * @param x Initial x-coordinate of the lower left front-side corner of the cuboid.
 * @param y Initial y-coordinate of the lower left front-side corner of the cuboid.
 * @param z Initial z-coordinate of the lower left front-side corner of the cuboid.
 * @param sizeX Number of particles along the x-axis.
 * @param sizeY Number of particles along the y-axis.
 * @param sizeZ Number of particles along the z-axis.
 * @param distance Distance between particles.
 * @param mass Mass of one particle.
 * @param velocityX Initial velocity along the x-axis.
 * @param velocityY Initial velocity along the y-axis.
 * @param velocityZ Initial velocity along the z-axis.
 * @param filename Path of file to which particles are written.
 */     
void generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char *filename);

/**
 * Generates a disc of particles and writes to the specified file.
 *
 * @param x x-coordinate of the center of the disc.
 * @param y y-coordinate of the center of the disc.
 * @param z z-coordinate of the center of the disc.
 * @param radius Radius of the disc in terms of the number of molecules.
 * @param MoleculesPerRadius Number of molecules along the radius.
 * @param distance Distance (mesh width) between the molecules.
 * @param mass Mass of each molecule.
 * @param velocityX Initial velocity along the x-axis for the whole disc.
 * @param velocityY Initial velocity along the y-axis for the whole disc.
 * @param velocityZ Initial velocity along the z-axis for the whole disc.
 * @param filename File to which the particles will be written.
 */
void ParticleGenerator::generateDisc(double x, double y, double z, double radius, size_t MoleculesPerRadius, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char* filename);
/**
 * @brief Logs a usage message.
 */
void logHelp();
};