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
            Cuboid cuboid;

            const std::array<double,3> velocity = {cuboid_xml.velocity().x(), cuboid_xml.velocity().y(), cuboid_xml.velocity().z()};
            const std::array<double,3> position = {cuboid_xml.position().x(), cuboid_xml.position().y(), cuboid_xml.position().z()};
            const std::array<unsigned,3> dimensions = {cuboid_xml.dimensions().x(), cuboid_xml.dimensions().y(), cuboid_xml.dimensions().z()};
            const double distance = cuboid_xml.distance();
            const double mass = cuboid_xml.mass();

            cuboid.setVelocity(velocity);
            cuboid.setPosition(position);
            cuboid.setDimensions(dimensions);
            cuboid.setDistance(distance);
            cuboid.setMass(mass);

            cuboids.push_back(cuboid);
        }
    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Cuboid-parsing.");
        spdlog::error(e.what());
        return;
    }
}

void XMLReader::readSimulation(const char* filename, std::string& baseName, unsigned& writeFrequency, double& start_time, double& end_time, double& delta_t){    
    try{
        std::unique_ptr<simulation> sim = simulation_(std::string{filename}, xml_schema::flags::dont_validate);

        baseName = sim->outputSettings().baseName();

        writeFrequency = sim->outputSettings().writeFrequency();

        start_time = sim->simulationParameters().start_t();

        end_time = sim->simulationParameters().end_t();

        delta_t = sim->simulationParameters().delta_t();

    }catch(const xml_schema::exception& e){
        spdlog::error("Error during Simulation-parsing.");
        spdlog::error(e.what());
        return;
    }
}