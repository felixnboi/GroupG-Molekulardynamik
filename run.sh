#!/bin/bash
#SBATCH -J my_program
#SBATCH --mail-user=ge47pov@mytum.de
#SBATCH --mail-type=ALL
#SBATCH -o ./%x.%j.%N.out
#SBATCH --clusters=serial
#SBATCH --partition=serial_std
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=800mb
#SBATCH --time=00:01:00
#SBATCH --export=NONE
#SBATCH -D ./build

# Load necessary modules
module load gcc/11.2.0
module load xerces-c/3.2.1
module load cmake

# Execute your program
./MolSim --xml ../input/Assignment2.xml -t