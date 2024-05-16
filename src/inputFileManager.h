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
 * @param filename1 The path of the file to be merged into.
 * @param filename2 The path of the file to be merged to filename1.
 */
void mergeFile(const char *filename1, const char *filename2);
/**
 * @brief Resets the generated input file to its initial state.
 * 
 * This function clears the contents of the given file and writes a header line followed by
 * a line specifying the initial number of particles (0).
 * 
 * @param filename The pah of file to be reset.
 */
void resetFile(const char *filename);

};