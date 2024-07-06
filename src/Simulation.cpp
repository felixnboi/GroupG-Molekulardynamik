#include "Simulation.h"

Simulation::Simulation(){
    std::array<std::string, 6> boundary = {};
    std::array<double, 3> domain = {};
    std::array<double, 3> domain_start = {0, 0, 0};
    simdata = SimData(std::string(""), std::string("MD_vtk"), 100, 0, 1000, 0.014, std::string(""), std::string("default"), 
    std::string("INFO"), boundary, 3, 2, domain, domain_start, 0);

    thermostat = Thermostat();
    thermostat_data = ThermostatData();
    checkpoint_data = CheckpointData(false, false, std::string(""), false, std::string(""));

    particles = nullptr;
    force = nullptr;

    input_file_user = "";
}

Simulation::~Simulation() {}

bool Simulation::initialize(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);
    const char* const short_ops = "v:i:gt";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"xml", required_argument, nullptr, 'x'},
        {"end", required_argument, nullptr, 'e'},
        {"start", required_argument, nullptr, 's'},
        {"log", required_argument, nullptr, 'l'},
        {"delta", required_argument, nullptr, 'd'},
        {"force", required_argument, nullptr, 'f'},
        {"dimensions", required_argument, nullptr, 'z'},
        {nullptr, no_argument, nullptr, 0}
    };

    int opt;

    const char* xml_file = "";

    // Parsing command line arguments
    while ((opt = getopt_long(argc, argv, short_ops, long_opts, nullptr)) != -1) {
        switch (opt) {
            
            case 'x':{
                xml_file = optarg;
                xml_flag = true;
                break;
            }
            case 't':{
                spdlog::set_level(spdlog::level::off);
                time_flag = true;
                break;
            }
            case 'h':{
                logHelp();
                return false;
            }
            case 'v':{
                cli_flag = true;
                if (isUnsignedInt(optarg)) {
                    simdata.setWriteFrequency(std::stoul(optarg));
                    break;
                } else {
                    spdlog::error("Invalid value for vtk iteration: {}", optarg);
                    logHelp();
                    return false;
                }
              }
            case 'l':{
                cli_flag = true;
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
                return false;
                }

            case 'g':{
                cli_flag = true;
                generate_flag = true;
                break;
            }

            case 'd':{
                cli_flag = true;
                if (isDouble(optarg)) {
                    simdata.setDeltaT(atof(optarg));
                    break;
                } else {
                    spdlog::error("Invalid argument for delta_t");
                    logHelp();
                    return false;
                }
            }

            case 'e':{
                cli_flag = true;
                if (isDouble(optarg)) {
                    simdata.setEndTime(atof(optarg));
                    break;
                } else {
                    spdlog::error("Invalid argument for end_time");
                    logHelp();
                    return false;
                }
            }

            case 's':{
                cli_flag = true;
                if (isDouble(optarg)) {
                    simdata.setStartTime(atof(optarg));
                    break;
                } else {
                    spdlog::error("Invalid argument for start_time");
                    logHelp();
                    return false;
                }
            }

            case 'i':{
                cli_flag = true;
                input_flag = true;
                input_file_user = optarg;
                break;
            }

            case 'f':{
                cli_flag = true;
                force_flag = true;
                if (*optarg == 'g') {
                    force = std::make_unique<Gravitational_Force>();
                    spdlog::info("Force set to Gravitational_Force");
                    break;
                } if (*optarg == 'l') {
                    force = std::make_unique<Lennard_Jones_Force>(lenJonesBoundaryFlags, periodicFlags);
                    spdlog::info("Force set to Lennard_Jones_Force");
                    break;
                } 
                    spdlog::error("Invalid argument for force");
                    logHelp();
                    return false;
            }

            case '?':{
                spdlog::error("Invalid option");
                logHelp();
                return false;
            }
        }
    }

    spdlog::info("Hello from MolSim for PSE!");

    if(xml_flag){

        if(xml_flag && cli_flag){
            spdlog::error("Please use either cli or xml!");
            logHelp();
            return false;
        }

        XMLReader xmlreader;
        xmlreader.readSimulation(xml_file, simdata);
        xmlreader.readThermostat(xml_file, thermostat_data);
        xmlreader.readCheckpoint(xml_file, checkpoint_data);

        if(thermostat_data.getThermostatFlag() && !thermostat_data.getInitTempFlag() && !thermostat_data.getTargetTemp()){
            spdlog::error("Either initial temperature or target termperature or both have to be set when using the thermostat");
            return false;
        }
        if(thermostat_data.getThermostatFlag() && !thermostat_data.getTargetTemp() && thermostat_data.getInitTemp()){
            thermostat_data.setTargetTemp(thermostat_data.getInitTemp());
        }

        thermostat.setThermostatData(thermostat_data);


        if(simdata.getLoglevel() == "OFF"){spdlog::set_level(spdlog::level::off);}
        if(simdata.getLoglevel() == "ERROR"){spdlog::set_level(spdlog::level::err);}
        if(simdata.getLoglevel() == "WARN"){spdlog::set_level(spdlog::level::warn);}
        if(simdata.getLoglevel() == "INFO"){spdlog::set_level(spdlog::level::info);}
        if(simdata.getLoglevel() == "DEBUG"){spdlog::set_level(spdlog::level::debug);}
        if(simdata.getLoglevel() == "TRACE"){spdlog::set_level(spdlog::level::trace);}

        for(int i = 0; i<6; i++){
            if(simdata.getBoundary()[i] == "outflow"){
                outflowFlags[i] = true;
                lenJonesBoundaryFlags[i] = false;
            }
            if(simdata.getBoundary()[i] == "reflecting"){
                outflowFlags[i] = false;
                lenJonesBoundaryFlags[i] = true;
            }
            if(simdata.getBoundary()[i] == "mirror"){
                outflowFlags[i] = false;
                lenJonesBoundaryFlags[i] = false;
            }
        }

        for(int i = 0; i<6 ; i+=2){
            if(simdata.getBoundary()[i] == "periodic"){
                if(simdata.getBoundary()[i+1] == "periodic"){
                    periodicFlags[i/2] = true;
                    outflowFlags[i] = false;
                    lenJonesBoundaryFlags[i] = false;
                    outflowFlags[i+1] = false;
                    lenJonesBoundaryFlags[i+1] = false;
                }else{
                    spdlog::error("Please make sure that parallel boundaries are either periodic or non-periodic");
                    return false;
                }
            }
            if(simdata.getBoundary()[i+1] == "periodic"){
                if(simdata.getBoundary()[i] == "periodic"){
                    periodicFlags[i/2] = true;
                    outflowFlags[i] = false;
                    lenJonesBoundaryFlags[i] = false;
                    outflowFlags[i+1] = false;
                    lenJonesBoundaryFlags[i+1] = false;
                }else{
                    spdlog::error("Please make sure that parallel boundaries are either periodic or non-periodic");
                    return false;
                }
            }
        }

        if(simdata.getAlgorithm() == "linkedcell"){
            particles = std::make_unique<ParticleContainerLinkedCell>(simdata.getDomain()[0], simdata.getDomain()[1], 
            simdata.getDomain()[2], simdata.getCutoffRadius());
            linkedcell_flag = true;
        }

        if(simdata.getAlgorithm() == "default"){
            particles = std::make_unique<ParticleContainerOld>();
        }

        if(simdata.getForceStr() == "gravitationalForce"){
            force = std::make_unique<Gravitational_Force>();
        }

        if(simdata.getForceStr() == "lennardJonesForce"){
            force = std::make_unique<Lennard_Jones_Force>(lenJonesBoundaryFlags, periodicFlags);
        }

    }else{

        particles = std::make_unique<ParticleContainerOld>();

        if (!force_flag) {
            spdlog::error("Didn't specify force. Terminating");
            logHelp();
            return false;
        }

        if (!generate_flag && input_flag) {
            simdata.setInputFile(input_file_user);
            spdlog::info("Using user defined input file");
        }

        if (generate_flag && input_flag) {
            inputFileManager::mergeFile("../input/generated-input.txt", input_file_user.c_str());
            spdlog::info("File {} merged into generated input file", input_file_user);
            simdata.setInputFile("../input/generated-input.txt");
        }

        if (generate_flag && !input_flag) {
            simdata.setInputFile("../input/generated-input.txt");
            spdlog::info("Using \"generated-input.txt\"");
        }

        if (!generate_flag && !input_flag) {
            simdata.setInputFile("../input/eingabe-sonne.txt");
            spdlog::info("Using \"eingabe-sonne.txt\"");
        }
    }


    // Check if start_time is after end_time
    if (simdata.getStartTime() > simdata.getEndTime()) {
        spdlog::error("Error: start_time should not be after end_time!");
        return false;
    }

    FileReader fileReader;
    fileReader.readFile(*particles, simdata.getInputFile().c_str(), simdata.getDomainStart());

    // checking if there are particles in the simulation
    if (particles->getParticles().empty()) {
        spdlog::error("Read zero particles from input file!");
        return false;
    }

    spdlog::info("end_time:{}, delta_t:{}, start_time:{}", simdata.getEndTime(), simdata.getDeltaT(), simdata.getStartTime());

    return true;
}

void Simulation::run() {
    auto start = std::chrono::high_resolution_clock::now();

    int iteration = 0;
    double current_time = 0;
    const double start_time = simdata.getStartTime();
    const double end_time = simdata.getEndTime();
    const double delta_t = simdata.getDeltaT();
    const unsigned write_frequency = simdata.getWriteFrequency();
    const double grav_constant = simdata.getGravConstant();

    bool thermostat_flag = thermostat.getThermostatData().getThermostatFlag();
    double initial_temp = thermostat.getThermostatData().getInitTemp();
    size_t n_thermostat = thermostat.getThermostatData().getNThermostat();

    if(thermostat_flag && initial_temp != 0.0){
        thermostat.initSystemTemperature(initial_temp, particles);
    }

    // Advance simulation time to start_time
    while (current_time < start_time) {
        calculateX();
        force->calculateF(*particles, linkedcell_flag, grav_constant);
        calculateV();
        current_time += delta_t;
        iteration++;
    }


    // Simulation loop
    if (time_flag) {
        while (current_time < end_time) {
            calculateX();
            force->calculateF(*particles, linkedcell_flag, grav_constant);
            calculateV();

            if(thermostat_flag && (iteration % n_thermostat == 0)){
                thermostat.scaleWithBeta(particles);
            }

            iteration++;
            current_time += delta_t;
        }
    } else {
        while (current_time < end_time) {
            calculateX();
            force->calculateF(*particles, linkedcell_flag, grav_constant);
            calculateV();

            if(thermostat_flag && (iteration % n_thermostat == 0)){
                thermostat.scaleWithBeta(particles);
            }

            // plotting particle positions only at intervals of iterations
            if (iteration % write_frequency == 0) {
                plotParticles(iteration);
            }

            spdlog::info("Iteration {} finished", iteration);

            current_time += delta_t;
            iteration++;
        }
    }
    if(checkpoint_data.getCheckpointFlag()){
        if(checkpoint_data.getCheckpointFileFlag()){
            CheckpointWriter::writeCheckpoint(*particles, checkpoint_data.getCheckpointFile().c_str());
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if(time_flag) {
        std::cout << "Execution time: " << elapsed.count() << " seconds\n";
        std::cout << "Molecule updates per second: " << iteration*particles->getParticleCount()/elapsed.count() << "\n";
    }

    spdlog::info("Output written. Terminating...");
}

void Simulation::calculateX() {
  double delta_t = simdata.getDeltaT();
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
    auto m = (*p)->getM(); ///< Mass of the particle.
    auto cur_x = (*p)->getX(); ///< Current position of the particle.
    auto cur_v = (*p)->getV(); ///< Current velocity of the particle.
    auto cur_F = (*p)->getF(); ///< Current force acting on the particle.
    std::array<double, 3> cur_x_dummy = {0,0,0}; ///< Dummy array to store new position components.

    // calculating new position components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_x_dummy[i] = cur_x[i] + delta_t * cur_v[i] + delta_t * delta_t * cur_F[i] / (2*m); 
    }
    // set the new position for the particle
    (*p)->setX(cur_x_dummy);
  }
  if(linkedcell_flag){
    ParticleContainerLinkedCell *LCContainer = dynamic_cast<ParticleContainerLinkedCell*>(particles.get());
    LCContainer->updateLoctions(outflowFlags, periodicFlags);
  }
}

void Simulation::calculateV() {
  double delta_t = simdata.getDeltaT();
  // iterating over all particles to calculate new positions
  for (auto p = particles->begin(); p != particles->end(); p++){
    auto m = (*p)->getM(); ///< Mass of the particle.
    auto cur_v = (*p)->getV(); ///< Current velocity of the particle.
    auto cur_F = (*p)->getF(); ///< Current force acting on the particle.
    auto old_F = (*p)->getOldF(); ///< Previous force acting on the particle.
    std::array<double, 3> cur_v_dummy = {0,0,0}; ///< Dummy array to store new velocity components.
    // calculating new velocity components for each dimension (x, y, z)
    for(int i = 0; i<3; i++){
      cur_v_dummy[i] = cur_v[i] + delta_t * (old_F[i] + cur_F[i]) / (2*m);
    }
    // set the new velocity for the particle
    (*p)->setV(cur_v_dummy);
  }
}


void Simulation::plotParticles(int iteration) {
  std::string baseName = simdata.getBaseName();

  outputWriter::VTKWriter writer; ///< The VTK writer object. 

  // initializing the VTK writer with the total number of particles.
  writer.initializeOutput(particles->getParticles().size()); 

  // iterating over each particle to plot its position
  for(const auto& p : particles->getParticles()){
    writer.plotParticle(*p);
  }

  // write the plotted particle positions to a VTK file
  writer.writeFile(baseName, iteration);
}

void Simulation::logHelp(){
  spdlog::info("Usage: \"[./MolSim --xml string [--help] [-t]] | [./MolSim [--help] [-t] [-g] [-i string] [-v int] [--log string] [--delta double] [--end double] [--start double] [--force char]\"");
  spdlog::info("For further information please read the README.md file at top level.");
  spdlog::info("Terminating...");
}