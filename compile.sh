#!/bin/bash

# Load necessary modules
module load cmake
module load gcc/11.2.0
module load xerces-c/3.2.1

# Create build directory if it does not exist
mkdir -p build

# Change to the build directory
cd build

# Configure the project
cmake ..

# Build the project
make