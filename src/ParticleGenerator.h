#pragma once

#include "Particle.h"
#include "utils/MaxwellBoltzmannDistribution.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

void generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ);