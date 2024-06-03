#include <Simulation.h>

Simulation::Simulation()
    : start_time(0), end_time(1000), delta_t(0.014), vtk_iteration(10), outflowFlags({false,false,false,false,false,false}), lenJonesBouneryFlags({true,true,true,true,true,true}),
      timing_enabled(false), force(nullptr), g_flag(false), i_flag(false), f_flag(false), t_flag(false), linkedCellsFlag(true) {}

Simulation::~Simulation() {}

//TODO inizialization of outflowFlags, lenJonesBouneryFlags and linkedCellsFlag

bool Simulation::initialize(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);
    const char* const short_ops = "v:i:gt";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"end", required_argument, nullptr, 'e'},
        {"start", required_argument, nullptr, 's'},
        {"log", required_argument, nullptr, 'l'},
        {"delta", required_argument, nullptr, 'd'},
        {"force", required_argument, nullptr, 'f'},
        {nullptr, no_argument, nullptr, 0}
    };

    int opt;
    particles = new ParticleContainerOld();

    // Parsing command line arguments
    while ((opt = getopt_long(argc, argv, short_ops, long_opts, nullptr)) != -1) {
        switch (opt) {
            case 't':{
                spdlog::set_level(spdlog::level::off);
                t_flag = true;
                break;
            }
            case 'h':{
                logHelp();
                return EXIT_SUCCESS;
            }
            case 'v':{
                // parsing vtk iteration
                if (isUnsignedInt(optarg)) {
                    vtk_iteration = std::stoul(optarg);
                } else {
                    spdlog::error("Invalid value for vtk iteration: {}", optarg);
                    logHelp();
                    return EXIT_FAILURE;
                }
              }
            case 'l':{
                // Parse logging level
                std::string tmp(optarg);
                if (tmp == std::string("OFF")) {
                    spdlog::set_level(spdlog::level::off);
                    spdlog::info("Logging level set to OFF");
                    break;
                } if (tmp == std::string("ERROR")) {
                    spdlog::set_level(spdlog::level::err);
                    spdlog::info("Logging level set to ERROR");
                    break;
                } if (tmp == std::string("WARN")) {
                    spdlog::set_level(spdlog::level::warn);
                    spdlog::info("Logging level set to WARN");
                    break;
                } if (tmp == std::string("INFO")) {
                    spdlog::set_level(spdlog::level::info);
                    spdlog::info("Logging level set to INFO");
                    break;
                } if (tmp == std::string("DEBUG")) {
                    spdlog::set_level(spdlog::level::debug);
                    spdlog::info("Logging level set to DEBUG");
                    break;
                } if (tmp == std::string("TRACE")) {
                    spdlog::set_level(spdlog::level::trace);
                    spdlog::info("Logging level set to TRACE");
                    break;
                }
                spdlog::error("Invalid logging level: {}", optarg);
                logHelp();
                return EXIT_FAILURE;
                }

            case 'g':{
                g_flag = true;
                break;
            }

            case 'd':{
                if (isDouble(optarg)) {
                    delta_t = atof(optarg);
                    break;
                } else {
                    spdlog::error("Invalid argument for delta_t");
                    logHelp();
                    return EXIT_FAILURE;
                }
            }

            case 'e':{
                if (isDouble(optarg)) {
                    end_time = atof(optarg);
                    break;
                } else {
                    spdlog::error("Invalid argument for end_time");
                    logHelp();
                    return EXIT_FAILURE;
                }
            }

            case 's':{
                if (isDouble(optarg)) {
                    start_time = atof(optarg);
                    break;
                } else {
                    spdlog::error("Invalid argument for start_time");
                    logHelp();
                    return EXIT_FAILURE;
                }
            }

            case 'i':{
                i_flag = true;
                input_file_user = optarg;
                break;
            }

            case 'f':{
                f_flag = true;
                if (*optarg == 'g') {
                    force = new Gravitational_Force();
                    spdlog::info("Force set to Gravitational_Force");
                    break;
                } if (*optarg == 'l') {
                    force = new Lennard_Jones_Force();
                    spdlog::info("Force set to Lennard_Jones_Force");
                    break;
                } 
                    spdlog::error("Invalid argument for force");
                    logHelp();
                    return EXIT_FAILURE;
            }

            case '?':{
                spdlog::error("Invalid option");
                logHelp();
                return EXIT_FAILURE;
            }
        }
    }

    spdlog::info("Hello from MolSim for PSE!");
    if (!f_flag) {
        spdlog::error("Didn't specify force. Terminating");
        logHelp();
        return EXIT_FAILURE;
    }

    if (!g_flag && i_flag) {
        input_file = input_file_user;
        spdlog::info("Using user defined input file");
    } 
    if (g_flag && i_flag) {
        inputFileManager::mergeFile("../input/generated-input.txt", input_file_user.c_str());
        spdlog::info("File {} merged into generated input file", input_file_user);
        input_file = "../input/generated-input.txt";
    }
    if (g_flag && !i_flag) {
        input_file = "../input/generated-input.txt";
        spdlog::info("Using \"generated-input.txt\"");
    }
    if (!g_flag && !i_flag) {
        input_file = "../input/eingabe-sonne.txt";
        spdlog::info("Using \"eingabe-sonne.txt\"");
    }

    // Check if start_time is after end_time
    if (start_time > end_time) {
        spdlog::error("Error: start_time should not be after end_time!");
        return EXIT_FAILURE;
    }

    FileReader fileReader;
    fileReader.readFile(*particles, input_file.c_str());

    // checking if there are particles in the simulation
    if (particles->getParticles().empty()) {
        spdlog::error("Failed to read Particles from input file!");
        return EXIT_FAILURE;
    }

    spdlog::info("end_time:{}, delta_t:{}, start_time:{}", end_time, delta_t, start_time);

    return true;
}

void Simulation::run() {
    double current_time = 0;
    int iteration = 0;

    // Advance simulation time to start_time
    while (current_time < start_time) {
        calculateX();
        force->calculateF(*particles, lenJonesBouneryFlags);
        calculateV();
        current_time += delta_t;
        iteration++;
    }

    // Simulation loop
    if (t_flag) {
        while (current_time < end_time) {
            calculateX();
            force->calculateF(*particles, lenJonesBouneryFlags);
            calculateV();
            iteration++;
            current_time += delta_t;
        }
    } else {
        while (current_time < end_time) {
            calculateX();
            force->calculateF(*particles, lenJonesBouneryFlags);
            calculateV();
            iteration++;

            // plotting particle positions only at intervals of iterations
            if (iteration % vtk_iteration == 0) {
                plotParticles(iteration);
            }
            // printing simulation progress
            spdlog::info("Iteration {} finished", iteration);
            // update simulation time
            current_time += delta_t;
        }
    }
// display output message and terminate the program
spdlog::info("Output written. Terminating...");
}

bool Simulation::isTimingEnabled() const {
    return timing_enabled;
}

void Simulation::cleanup(){
    delete force;
    delete particles;
}

void Simulation::calculateX() {
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
    auto m = p->getM(); ///< Mass of the particle.
    auto cur_x = p->getX(); ///< Current position of the particle.
    auto cur_v = p->getV(); ///< Current velocity of the particle.
    auto cur_F = p->getF(); ///< Current force acting on the particle.
    std::array<double, 3> cur_x_dummy = {0,0,0}; ///< Dummy array to store new position components.

    // calculating new position components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_x_dummy[i] = cur_x[i] + delta_t * cur_v[i] + delta_t * delta_t * cur_F[i] / (2*m); 
    }
    // set the new position for the particle
    p->setX(cur_x_dummy);
  }
  if(linkedCellsFlag){
    ParticleContainerLinkedCell *LCContainer = dynamic_cast<ParticleContainerLinkedCell*>(particles);
    LCContainer->updateLoctions(outflowFlags);
  }

}

void Simulation::calculateV() {
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
    auto m = p->getM(); ///< Mass of the particle.
    auto cur_v = p->getV(); ///< Current velocity of the particle.
    auto cur_F = p->getF(); ///< Current force acting on the particle.
    auto old_F = p->getOldF(); ///< Previous force acting on the particle.
    std::array<double, 3> cur_v_dummy = {0,0,0}; ///< Dummy array to store new velocity components.
    // calculating new velocity components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
    }
    // set the new velocity for the particle
    p->setV(cur_v_dummy);
  }
}


void Simulation::plotParticles(int iteration) {

  std::string out_name("MD_vtk"); ///< The base name of the VTK file to be written.

  outputWriter::VTKWriter writer; ///< The VTK writer object. 
  // initializing the VTK writer with the total number of particles.
  writer.initializeOutput(particles->getParticles().size()); 
  // iterating over each particle to plot its position
  for(const auto& p : particles->getParticles()){
    writer.plotParticle(p);
  }
  // write the plotted particle positions to a VTK file
  writer.writeFile(out_name, iteration);
}

void Simulation::logHelp(){
  spdlog::info("Usage: \"./MolSim [--help] [-g] [-i string] [-v int] [--log string] [--delta double] [--end double] [--start double] --force char\"");
  spdlog::info("For further information please read the README.md file at top level.");
  spdlog::info("Terminating...");
}