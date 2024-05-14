# GroupG

## Members:

    Felix Guan

    Robert Täger

    Maria Lelyukh


## Dependencies: 

    gcc/g++ compiler: the default version under most linux distributions should be sufficient

    xerces library: run sudo apt install libxerces-c-dev in your terminal
    
    cmake: run sudo apt install cmake in your terminal

    paraview (for visualizing): run sudo apt install paraview in your terminal

## Build instructions:

    1.clone the repository

    2.mkdir build in the repo

    3.cd build

    4.run cmake ..

    5.run make or make doc_doxygen for documentation

    6.Execute the executable with: "Usage: ./MolSim <path/to/input/file> [[end_time delta_t] | [end_time delta_t start_time]]". 

      Please make sure to pass numerical values to end_time delta_t and start_time. Also pay attention to the order of the arguments. If a start_time is given the program will only plot the position of the particles from start_time to end_time.

      Example for execution: "./MolSim /path/to/input/file 1000 0.014" or "./MolSim /path/to/input/file 10000 0.014 5000".



