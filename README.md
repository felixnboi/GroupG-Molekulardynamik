# GroupG

Welcome to our repository for *PSE Molecular Dynamics* at TUM  

## Members

Felix Guan  
Robert Täger  
Maria Lelyukh  

## Dependencies

gcc/g++ compiler: the default version under most linux distributions should be sufficient  
xerces library: to install run *sudo apt install libxerces-c-dev*  
cmake: to install run *sudo apt install cmake*
xsdcxx compiler: to install run *sudo apt install xsdcxx*  
paraview (for visualizing): to install run *sudo apt install paraview*  

## Build instructions
To build the project run the following commands:
```
mkdir build
cd build
cmake ..
make
```
To build doxygen documentation:
```
make doc_doxygen
``` 

## Execution instructions:
After making there will be two executables in the build folder. One called *MolSim* which runs the main program and another called *ParticleGenerator* which is for generating Particles. ParticleGenerator takes parameters to define a cuboid, computes the particles within this cuboid and writes these Particles to a file called *generated-input.txt* which is located in the */input* folder. Always make sure to execute *ParticleGenerator* first.  

#### MolSim:
The MolSim executable uses the *getopt_long()* function of the standard library to parse command line arguments. The usages are as follows:  
Usage: *"./MolSim --xml string [-t] [--help]"* or *"./MolSim [--help] [-g] [-t] [-i string] [-v int] [--log string] [--delta double] [--end double] [--start double] [--force char]"*  

Primary usage(recommended):  

*"--xml string"*: **mandatory** Give the path to the .xml-file to be used for parameter parsing.  
*"--help"*: **optional** If set a usage message is logged and program terminates.  
*"-t"*: **optional** If set the program execution time will be measured. Measurement takes place with no vtk-output and no logging.  

Example: ./MolSim --xml ../input/Assignment2.xml  


Please make sure that this .xml-file can be validated against [XMLFormat.xsd](/src/io/input/XMLFormat.xsd). This usage has to be used for assignments after assignment2.  
For backwards compatability with assignment1 and assignment2 the secondary usage can be used:  

<details>
<summary>secondary usage</summary>  

*"--help"*: **optional** If set a usage message is logged and program terminates.  
*"-g"*: **optional** If set the program uses the *"generated-input.txt"* file in the */input* folder.  
*"-t"*: **optional** If set the program execution time will be measured. Measurement takes place with no vtk-output and no logging.  
*"-i string"*: **optional** Defines the path of the input file.  
*"-v int"*: **optional** Defines the iterations of output. For example if the argument is *100*, every 100th iteration a output will be generated.  
*"--log string"*: **optional** Defines the loglevel of the program. *"OFF"*, *"ERROR"*, *"WARN"*, *"INFO"*, *"DEBUG"* and *"TRACE"* are valid arguments.  
*"--delta double"*: **optional** Defines the time-step of computing.  
*"--end double"*: **optional** Defines the end-time of computing.  
*"--force char"*: **madatory** Either *'l* or *'g'* can be parsed as argument. If *'l'* then Lennard-Jones Force is used for computing. If *'g'* then Gravitaitonal Force is used for computing.  

**NOTICE**: For all optional command line options that take an argument there are default values if not specified by user via command line.   
If both *"-i string"* and *"-g"* are set both the *"generated-input.txt"* and the file at the path provided in *string* are used (i.e. the particles from both files will be used). The particles from the file at the path provided at *string* will also be written to *"generated-input.txt"*.  
If none of both are set the *"eingabe-sonne.txt"* file at path */input* is used.  

Example: ./MolSim -g --force l --delta 0.0002 --end 5  

</details>  


#### ParticleGenerator:
The ParticleGenerator executable also uses the *getopt_long()* function of the standard library to parse command line arguments. The usages are as follows:  
Usage: *"./ParticleGenerator --xml string [--help] [-t]"* or *"./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double"*

Primary usage:  

*"--xml string"*: **mandatory** Give the path to the .xml-file to be used for parameter parsing.  
*"--help"*: **optional** If set a usage message is logged and program terminates.  

Example: ./MolSim --xml ../input/Assignment2.xml  

Please make sure that this .xml-file can be validated against [XMLFormat.xsd](/src/io/input/XMLFormat.xsd). This usage has to be used for assignments after assignment2.  
For backwards compatability with assignment1 and assignment2 the second usage can be used:  

<details>
<summary> secondary usage </summary>  

*"--help"*: **optional** If set a usage message is logged and program terminates.  
*"-s"*: **optional** If set the particles in the already exsisting *generated-input.txt* file won't be overwritten and the new particles will be added as well. Please be sure that a *generated-input.txt* file has already been created when setting this option.  
*"-m double"*: **mandatory.** Defines the mass of each particle in the cuboid.  
*"-d double"*: **mandatory.** Defines the distance between the particles.  
*"-x double"*: **mandatory.** Defines x-coordinate of the lower left front side corner of the cuboid.  
*"-y double"*: **mandatory.** Defines y-coordinate of the lower left front side corner of the cuboid.  
*"-z double"*: **mandatory.** Defines z-coordinate of the lower left front side corner of the cuboid.  
*"--sizeX int"*: **mandatory.** Defines number of particles in x-direction.  
*"--sizeY int"*: **mandatory.** Defines number of particles in y-direction.  
*"--sizeZ int"*: **mandatory.** Defines number of particles in z-direction.  
*"--velocityX double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in x-direction.  
*"--velocityY double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in y-direction.  
*"--velocityZ double"*: **mandatory.** Defines starting velocity of each particle in the cuboid in z-direction.  
*"--log string"*: **optional** Defines the loglevel of the program. *"OFF"*, *"ERROR"*, *"WARN"*, *"INFO"*, *"DEBUG"* and *"TRACE"* are valid arguments.  

Example: ./ParticleGenerator -m 1 -d 1.1225 -x 0 -y 0 -z 0 --sizeX 40 --sizeY 8 --sizeZ 1 --velocityX 0 --velocityY 0 --velocityZ 0  

</details>  

## Visualizing with paraview:

It is possible to visualize the output with paraview. Simply open the .vtu-output-files in paraview and configure to your preferred visualization.

## Running Tests:
    
Commands to run tests after making:
```
./tests/tests
```
or
```
ctest
```