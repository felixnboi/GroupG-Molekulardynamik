/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"
<<<<<<< HEAD
=======
#include "ParticleContainers/ParticleContainer.h"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
>>>>>>> assignment2

#include <list>

class FileReader {

public:
  FileReader();
  virtual ~FileReader();

  void readFile(std::list<Particle> &particles, char *filename);
};
