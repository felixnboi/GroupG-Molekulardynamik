#include "ParticleGenerator.h"
#include <cmath>

void ParticleGenerator::generateCuboid(const Cuboid& cuboid, const char *filename) {
    std::fstream input_file;
    std::string tmp_string;

    std::array<double, 3> position = cuboid.getPosition();
    std::array<double, 3> velocity = cuboid.getVelocity();
    std::array<unsigned, 3> dimensions = cuboid.getDimensions();
    double mass = cuboid.getMass();
    double distance = cuboid.getDistance();
    
    // spdlog::info("Generating cuboid with parameters: x={}, y={}, z={}, sizeX={}, sizeY={}, sizeZ={}, distance={}, mass={},
    //  velocityX={}, velocityY={}, velocityZ={}", position[0], position[1], position[2], dimensions[0], dimensions[1], dimensions[2], 
    //  distance, mass, velocity[0], velocity[1], velocity[2]);
    
    input_file.open(filename,std::ios::in|std::ios::out);
    std::streampos current = input_file.tellp();

    while (tmp_string.empty() or tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }

    input_file.seekp(current);
    int numParticles = std::stoi(tmp_string)+dimensions[0]*dimensions[1]*dimensions[2];
    input_file << numParticles;
    input_file.close();
    
    double yClone = position[1];
    double zClone = position[2];
    
    double averageBrownianMotion = 0.1;
    input_file.open(filename,std::ios::in|std::ios::out|std::ios::app);
    for (size_t i = 0; i < dimensions[0]; i++)
    {
        for (size_t j = 0; j < dimensions[1]; j++)
        {
            for (size_t k = 0; k < dimensions[2]; k++) {
                std::array<double, 3> brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrownianMotion, 2);
                input_file << position[0] << " " << position[1] << " " << position[2] << " " << velocity[0] + brownianMotion[0] << " " << velocity[1] + brownianMotion[1] << " " << velocity[2] + brownianMotion[2] << " " << mass << "\n";         
                position[2] += distance;
            }
            position[2] = zClone;
            position[1] += distance;
        }
        position[1] = yClone;
        position[0] += distance;
    }
    input_file.close();
}

void ParticleGenerator::generateDisc(double x, double y, double z, double radius, size_t MoleculesPerRadius, 
double distance, double mass, double velocityX, double velocityY, double velocityZ, const char* filename) {
    spdlog::info("Generating disc with parameters: x={}, y={}, z={}, radius={}, MoleculesPerRadius={}, distance={}, mass={}, velocityX={}, velocityY={}, velocityZ={}", x, y, z, radius, MoleculesPerRadius, distance, mass, velocityX, velocityY, velocityZ);
    
    std::fstream input_file(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!input_file.is_open()) {
        spdlog::error("Failed to open file {}", filename);
        return;
    }
    // skipping comments and getting the number of particles
    std::string tmp_string;
    std::streampos current = input_file.tellp();
    while (tmp_string.empty() || tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }
    input_file.seekp(current);
    int numParticles = std::stoi(tmp_string) + MoleculesPerRadius * MoleculesPerRadius;
    input_file << numParticles;
    input_file.close();

    input_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!input_file.is_open()) {
        spdlog::error("Failed to open file {}", filename);
        return;
    }
    // Generate particles within the specified radius
    for (size_t i = 0; i < MoleculesPerRadius; ++i) {
        for (size_t j = 0; j < MoleculesPerRadius; ++j) {
            double size_x = i * distance;
            double size_y = j * distance;
            double distanceFromCenter = std::sqrt(size_x * size_x + size_y * size_y);

            if (distanceFromCenter <= radius) {
                double particleX = x + size_x - radius;
                double particleY = y + size_y - radius;
                double particleZ = z;

                input_file << particleX << " " << particleY << " " << particleZ << " "
                           << velocityX << " " << velocityY << " " << velocityZ << " "
                           << mass << "\n";
            }
        }
    }

    input_file.close();
}

void ParticleGenerator::logHelp(){
    spdlog::info("Usage: \"./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double\"");
    spdlog::info("For further information please read the README.md file at top level.");
    spdlog::info("Terminating...");
}
