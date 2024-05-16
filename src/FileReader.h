/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Particle.h"
#include "ParticleContainer.h"

#include <list>

/**
 * @brief This class provides functionality to read particle data from a file and populate a ParticleContainer.
 */
class FileReader {

public:
    /**
     * @brief Default constructor.
     */
  FileReader();
  /**
     * @brief Destructor.
     */
  virtual ~FileReader();

/**
     * @brief Reads particle data from a file and populates a ParticleContainer.
     * 
     * This function reads particle data from the specified file and transfers it into the provided ParticleContainer.
     * The file format is expected to contain lines representing particles, with each line containing
     * the x, y, and z coordinates followed by the velocity components (vx, vy, vz) and the mass (m) of the particle.
     * 
     * @param particles The ParticleContainer to fill with the read particle data.
     * @param filename The path to the file containing the particle data.
     */
  void readFile(ParticleContainer &particles, const char *filename);
};
