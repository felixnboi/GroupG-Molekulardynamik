#!/bin/bash
#SBATCH --job-name=my_program
#SBATCH --mail-user=xxfelixg.xx@gmail.com
#SBATCH --mail-type=END
#SBATCH --output=my_program_%j.out
#SBATCH --error=my_program_%j.err
#SBATCH --clusters=cm2_tiny
#SBATCH --partition=cm2_tiny  # Specify the partition (queue) to submit to
#SBATCH --nodes=1             # Number of nodes
#SBATCH --ntasks-per-node=1   # Number of tasks (MPI processes)
#SBATCH --cpus-per-task=1     # Number of CPU cores per task
#SBATCH --mem=4G              # 4 GB of memory per node
#SBATCH --time=02:00:00       # Time limit hrs:min:sec
#SBATCH --export=NONE
#SBATCH -D ./build/

# Load necessary modules
module load gcc/11.2.0
module load xerces-c/3.2.1
module load cmake

# Execute your program
./MolSim --xml ../input/Assignemt4_task_2_2.xml -t