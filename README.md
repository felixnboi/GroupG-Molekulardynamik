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

>6. To execute the executable: Run *./MolSim* in the build folder. 
It is also possible to parse arguments via command line. Usage: */MolSim [-s start_time] [-e end_time] [-d delta_t] [-f path/to/inputfile]*
Example: "*./MolSim -f /../input/eingabe-sonne.txt -s 1000 -d 0.014*".
If no additional arguments are parsed via command line as described a set of defautl values is used.

## Visualizing with paraview:

>It is possible to visualize the output via paraview. Simply open the .vtu-output-files in paraview and configure to your preferred visualization.

## Running Tests:
    
>After building run *ctest* in the build folder or navigate to the tests folder in the build folder and execute with *./tests*. 


