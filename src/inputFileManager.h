#pragma once

#include "spdlog/spdlog.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
/**
 * @brief A namespace for managing input files.
 */
namespace inputFileManager{
/**
 * @brief Merges the contents of the specified file into the generated input file.
 * 
 * The function reads the first line of the specified file, which should contain the number of particles,
 * and appends the particles from the file into the generated input file.
 * 
 * @param filename The name of the file to be merged.
 */
void mergeFile(const char *filename1,const char *filename2);
/**
 * @brief Resets the generated input file to its initial state.
 * 
 * This function clears the contents of the generated input file and writes a header line followed by
 * a line specifying the initial number of particles (0).
 */
void resetFile(const char *filename);

};