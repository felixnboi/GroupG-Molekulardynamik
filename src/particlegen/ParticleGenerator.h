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
 *@brief Tests if this string is a double 
 */ 
bool isDouble(char* str); 

/**
 *@brief Tests if this string is an int 
 */ 
bool isInteger(char* str);

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
 */     

void generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char *filename);

/**
 * @brief Logs a usage message.
 */
void logHelp();

};