#include "XMLReader.h"

using namespace xercesc;

XMLReader::XMLReader() = default;

XMLReader::~XMLReader() = default;

void XMLReader::readCuboids(const char* filename, std::vector<Cuboid>& cuboids){
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

            cuboids.push_back(Cuboid(position, velocity, dimensions, distance, mass));
        }
    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Cuboid-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readDiscs(const char* filename, std::vector<Disc>& discs){
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

            discs.push_back(Disc(position, velocity, radius, distance, mass));
        }

    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Disc-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readSimulation(const char* filename, bool& generate_flag, std::string& inputFile, std::string& baseName, 
unsigned& writeFrequency, double& start_time, double& end_time, double& delta_t, std::string& force_str, std::string& algorithm, 
std::string& loglevel, std::array<std::string, 6>& boundary, double& cutoff_radius, std::array<double, 3>& domain){    
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        inputFile = sim->inputSettings().inputFile();
        baseName = sim->outputSettings().baseName();
        writeFrequency = sim->outputSettings().writeFrequency();
        start_time = sim->simulationParameters().start_t();
        end_time = sim->simulationParameters().end_t();
        delta_t = sim->simulationParameters().delta_t();
        force_str = sim->simulationParameters().force();
        algorithm = sim->simulationParameters().algorithm();
        loglevel = sim->simulationParameters().loglevel();

        boundary[0] = sim->boundaries().left();
        boundary[1] = sim->boundaries().right();
        boundary[2] = sim->boundaries().bottom();
        boundary[3] = sim->boundaries().top();
        boundary[4] = sim->boundaries().rear();
        boundary[5] = sim->boundaries().front();

        cutoff_radius = sim->simulationParameters().cutoff_radius();        

        domain[0] = sim->simulationParameters().domain().x();
        domain[1] = sim->simulationParameters().domain().y();
        domain[2] = sim->simulationParameters().domain().z();

    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Simulation-parsing.");
        spdlog::error(e.what());
        return;
    }
}