/**
 * @file inputFileMerger.h
 * @brief Contains the declaration of the inputFileMerger namespace and its functions.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief The inputFileMerger namespace contains functions for merging data from an input file with another file.
 */
namespace inputFileMerger{
 /**
     * @brief Merges the content of the specified file with the input file.
     * 
     * This function reads the content of the input file and another specified file,
     * then merges them together. It updates the number of particles in the input file
     * based on the content of the merging file.
     * 
     * @param filename The name of the file to merge with the input file.
     */
void mergeWithInputFile(char *filename);

};