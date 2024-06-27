#include "XMLReader.h"

using namespace xercesc;

XMLReader::XMLReader() = default;

XMLReader::~XMLReader() = default;

void XMLReader::readCuboids(const char* filename, std::vector<Cuboid>& cuboids, double epsilon, double sigma){
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        const simulation::cuboid_sequence& cuboid_sequence = sim->cuboid();

        std::vector<simulation::cuboid_type> cuboid_vector(cuboid_sequence.begin(), cuboid_sequence.end());

        for(const auto& cuboid_xml : cuboid_vector){

            const std::array<double, 3> velocity = {cuboid_xml.velocity().x(), cuboid_xml.velocity().y(), cuboid_xml.velocity().z()};
            const std::array<double, 3> position = {cuboid_xml.position().x(), cuboid_xml.position().y(), cuboid_xml.position().z()};
            const std::array<unsigned, 3> dimensions = {cuboid_xml.dimensions().x(), cuboid_xml.dimensions().y(), cuboid_xml.dimensions().z()};
            const double distance = cuboid_xml.distance();
            const double mass = cuboid_xml.mass();
            const double brownian_motion = cuboid_xml.brownian_motion();
            const double epsilon = cuboid_xml.epsilon();
            const double sigma = cuboid_xml.sigma();
            const double brownian_motion_dimension = cuboid_xml.brownian_motion_dimension();
            const int type = cuboid_xml.type();
            const bool is_outer = cuboid_xml.is_outer();

            cuboids.push_back(Cuboid(position, velocity, dimensions, distance, mass, brownian_motion, epsilon, sigma, 
            brownian_motion_dimension, type, is_outer));
        }
    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Cuboid-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readDiscs(const char* filename, std::vector<Disc>& discs, double epsilon, double sigma){
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        const simulation::disc_sequence& disc_sequence = sim->disc();

        std::vector<simulation::disc_type> disc_vector(disc_sequence.begin(), disc_sequence.end());

        for(const auto& disc_xml : disc_vector){

            const std::array<double, 3> position = {disc_xml.position().x(), disc_xml.position().y(), disc_xml.position().z()};
            const std::array<double, 3> velocity = {disc_xml.velocity().x(), disc_xml.velocity().y(), disc_xml.velocity().z()};

            const int radius = disc_xml.radius();
            const double distance = disc_xml.distance();
            const double mass = disc_xml.mass();
            const double epsilon = disc_xml.epsilon();
            const double sigma = disc_xml.sigma();
            const double brownian_motion_dimension = disc_xml.brownian_motion_dimension();
            const int type = disc_xml.type();

            discs.push_back(Disc(position, velocity, radius, distance, mass, epsilon, sigma, brownian_motion_dimension, type, false));
        }

    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Disc-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readSimulation(const char* filename, SimData& simdata){    
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        simdata.setInputFile(sim->inputSettings().inputFile());
        simdata.setBaseName(sim->outputSettings().baseName());
        simdata.setWriteFrequency(sim->outputSettings().writeFrequency());
        simdata.setStartTime(sim->simulationParameters().start_t());
        simdata.setEndTime(sim->simulationParameters().end_t());
        simdata.setDeltaT(sim->simulationParameters().delta_t());
        simdata.setForceStr(sim->simulationParameters().force());
        simdata.setAlgorithm(sim->simulationParameters().algorithm());
        simdata.setLoglevel(sim->simulationParameters().loglevel());
        simdata.setWallsFlag(sim->simulationParameters().walls_flag());

        const std::array<std::string, 6> boundary = {sim->boundaries().left(), sim->boundaries().right(), sim->boundaries().bottom(), 
        sim->boundaries().top(), sim->boundaries().rear(), sim->boundaries().front()};

        simdata.setBoundary(boundary);

        simdata.setCutoffRadius(sim->simulationParameters().cutoff_radius());

        const std::array<double, 3> domain = {sim->simulationParameters().domain().x(), sim->simulationParameters().domain().y(), 
        sim->simulationParameters().domain().z()};        

        simdata.setDomain(domain);

        const std::array<double , 3> domain_start = {sim->simulationParameters().domain_start().x(), sim->simulationParameters().domain_start().y(), 
        sim->simulationParameters().domain_start().z()};
        
        simdata.setDomainStart(domain_start);

        simdata.setGravConstant(sim->simulationParameters().grav_constant());

    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Simulation-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readThermostat(const char* filename, ThermostatData& thermostatdata){
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        if(sim->thermostat().present()){
            thermostatdata.setThermostatFlag(true);
            if(sim->thermostat().get().T_init().present()){
                thermostatdata.setInitTempFlag(true);
                thermostatdata.setInitTemp(sim->thermostat().get().T_init().get());
            }else{
                thermostatdata.setInitTempFlag(false);
            }
            if(sim->thermostat().get().T_target().present()){
                thermostatdata.setTargertTempFlag(true);
                thermostatdata.setTargetTemp(sim->thermostat().get().T_target().get());
            }else{
                thermostatdata.setTargertTempFlag(false);
            }
            if(sim->thermostat().get().T_diff().present()){
                thermostatdata.setMaxDeltaTemp(sim->thermostat().get().T_diff().get());
            }else{
                thermostatdata.setMaxDeltaTemp(std::numeric_limits<double>::infinity());
            }
            thermostatdata.setBrownianMotionDimension(sim->thermostat().get().brownian_motion_dimension());
            thermostatdata.setNThermostat(sim->thermostat().get().n_thermostat());
        }else{
            thermostatdata.setThermostatFlag(false);
        }
    }
    catch(const xml_schema::exception& e){
        spdlog::error("Error during Thermostat-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readCheckpoint(const char* filename, CheckpointData& checkpointdata){
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        if(sim->checkpoint().present()){
            checkpointdata.setCheckpointFlag(true);
            if(sim->checkpoint().get().checkpoint_file().present()){
                checkpointdata.setCheckpointFileFlag(true);
                checkpointdata.setCheckpointFile(sim->checkpoint().get().checkpoint_file().get());
            }else{
                checkpointdata.setCheckpointFileFlag(false);
            }
            if(sim->checkpoint().get().merge_file().present()){
                checkpointdata.setMergeFileFlag(true);
                checkpointdata.setMergeFile(sim->checkpoint().get().merge_file().get());
            }else{
                checkpointdata.setMergeFileFlag(false);
            }
            
        }else{
            checkpointdata.setCheckpointFlag(false);
        }


    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Checkpoint-parsing.");
        spdlog::error(e.what());
        return;
    }
}