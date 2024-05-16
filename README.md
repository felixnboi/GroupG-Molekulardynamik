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
>Usage: *"./MolSim [--help] [-g] [-i string] [-v int] [--log string] [--delta double] [--end double] [--start double] [--force char]"*

>*"--help"*: **optional** If set a usage message is logged and program terminates.  
>*"-g"*: **optional** If set the program uses the *"generated-input.txt"* file in the */input* folder.  
>*"-i string"*: **optional** Defines the path of the input file.  
>*"-v int"*: **optional** Defines the iterations of output. For example if the argument is *100*, every 100th iteration a output will be generated.  
>*"--log string"*: **optional** Defines the loglevel of the program. *"OFF"*, *"ERROR"*, *"WARN"*, *"INFO"*, *"DEBUG"* and *"TRACE"* are valid arguments.  
>*"--delta double"*: **optional** Defines the time-step of computing.   
>*"--end double"*: **optional** Defines the end-time of computing.  
>*"--force char"*: **madatory** Either *'l* or *'g'* can be parsed as argument. If *'l'* then Lennard-Jones Force is used for computing. If *'g'* then Gravitaitonal Force is used for computing.  

>**NOTICE**: For all optional command line options that take an argument there are default values if not specified by user via command line.   
If both *"-i string"* and *"-g"* are set both the *"generated-input.txt"* and the file at the path provided in *string* are used (i.e. the particles from both files will be used). The particles from the file at the path provided at *string* will also be written to *"generated-input.txt"*.  
If none of both are set the *"eingabe-sonne.txt"* file at path */input* is used.


#### ParticleGenerator:
>The ParticleGenerator executable also uses the *getopt_long()* function of the standard library to parse command line arguments. The usage is as follows:  
>Usage: *"./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double"*

>*"--help"*: **optional** If set a usage message is logged and program terminates.  
>*"-s"*: **optional** If set the particles in the already exsisting *generated-input.txt* file won't be overwritten and the new particles will be added as well. Please be sure that a *generated-input.txt* file has already been created when setting this option.
>*"-m double"*: **mandatory.** Defines the mass of the particles in the cuboid.  
>*"-d double"*: **mandatory.** Defines the distance between two particles.  
>*"-x double"*: **mandatory.** Defines starting point x-coordinate of the cuboid.  
>*"-y double"*: **mandatory.** Defines starting point y-coordinate of the cuboid.  
>*"-z double"*: **mandatory.** Defines starting point z-coordinate of the cuboid.  
>*"--sizeX int"*: **mandatory.** Defines length of cuboid in x-direction.  
>*"--sizeY int"*: **mandatory.** Defines length of cuboid in y-direction.  
>*"--sizeZ int"*: **mandatory.** Defines length of cuboid in z-direction.  
>*"--velocityX double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in x-direction.  
>*"--velocityY double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in y-direction.  
>*"--velocityZ double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in z-direction.  
>*"--log string"*: **optional** Defines the loglevel of the program. *"OFF"*, *"ERROR"*, *"WARN"*, *"INFO"*, *"DEBUG"* and *"TRACE"* are valid arguments.  

## Visualizing with paraview:

>It is possible to visualize the output via paraview. Simply open the .vtu-output-files in paraview and configure to your preferred visualization.

## Running Tests:
    
>After building run *ctest* in the build folder or navigate to the tests folder in the build folder and execute with *./tests*. 


