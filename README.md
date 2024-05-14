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

>6. To execute the executable: Run *./MolSim <path/to/input/file> [end_time delta_t]  [end_time delta_t start_time]* in the build folder. 
Please make sure to pass numerical values to end_time delta_t and start_time. Also pay attention to the order of the arguments. If a start_time is given the program will only plot the position of the particles from start_time to end_time.
Example for execution: "*./MolSim /../input/eingabe-sonne.txt 1000 0.014*" or "*./MolSim /../input/eingabe-sonne.txt 10000 0.014 5000*".

## Visualizing with paraview:

>It is possible to visualize the output via paraview. Simply open the .vtu-output-files in paraview and configure to your preferred visualization.

## Running Tests:
    
>After building run *ctest* in the build folder or navigate to the tests folder in the build folder and execute with *./tests*. 


