#pragma once

#include "../Particle.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../inputFileManager.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <getopt.h>

namespace ParticleGenerator{

/**
 *test if this string is a double 
 */ 
bool isDouble(char* str); 

/**
 *test if this string is a int 
 */ 
bool isInteger(char* str);

void generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ);

};