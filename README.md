# GroupG

>Welcome to our repository for *PSE Molecular Dynamics* at TUM 

## Members:

>Felix Guan

>Robert Täger

>Maria Lelyukh


## Dependencies: 

>gcc/g++ compiler: the default version under most linux distributions should be sufficient

>xerces library: to install run *sudo apt install libxerces-c-dev* 
    
>cmake: to install run *sudo apt install cmake*

>paraview (for visualizing): to install run *sudo apt install paraview*

## Build instructions:

>1. clone the repository

>2. *mkdir build* in the repo

>3. *cd build*

>4. run *cmake ..*

>5. run *make* for building executable and *make doc_doxygen* for building documentation

## Execution instructions:
>After making there will be two executables in the build folder. One called *MolSim* which runs the main program and another called *ParticleGenerator* which is for generating Particles. ParticleGenerator takes parameters to define a cuboid, computes the particles within this cuboid and writes these Particles to a file called *generated-input.txt* which is located in the */input* folder. 

#### MolSim:
>The MolSim executable uses the *getopt_long()* function of the standard library to parse command line arguments. The usage is as follows:


#### ParticleGenerator:
>The ParticleGenerator executable also uses the *getopt_long()* function of the standard library to parse command line arguments. The usage is as follows:  
>Usage: *"./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double"*

>*"--help"*: If set a usage message is logged and program terminates.  
>*"-s"*: If set the particles in the already exsisting *generated-input.txt*   file won't be overwritten and the new particles will be added as well.
>*"-m double"*: **mandatory.** Defines the mass of the particles in the cuboid.
>*"-d double"*: **mandatory.** Defines the distance between two particles.
>*"-x double"*: **mandatory.** Defines starting point x-coordinate of the cuboid.
>*"-y double"*: **mandatory.** Defines starting point y-coordinate of the cuboid.
>*"-z double"*: **mandatory.** Defines starting point z-coordinate of the cuboid.
>*"--sizeX int"*: **mandatory.** Defines length of cuboid in x-direction.
>*"--sizeY int"*: **mandatory.** Defines length of cuboid in y-direction.
>*"--sizeZ int"*: **mandatory.** Defines length of cuboid in z-direction.
>*"--velodityX double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in x-direction.
>*"--velodityY double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in y-direction.
>*"--velodityZ double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in z-direction.


## Visualizing with paraview:

>It is possible to visualize the output via paraview. Simply open the .vtu-output-files in paraview and configure to your preferred visualization.

## Running Tests:
    
>After building run *ctest* in the build folder or navigate to the tests folder in the build folder and execute with *./tests*. 


