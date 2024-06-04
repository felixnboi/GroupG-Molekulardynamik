#include "ParticleGenerator.h"
#include <cmath>

void ParticleGenerator::generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char *filename) {
    std::fstream input_file;
    std::string tmp_string;
    
    spdlog::info("Generating cuboid with parameters: x={}, y={}, z={}, sizeX={}, sizeY={}, sizeZ={}, distance={}, mass={}, velocityX={}, velocityY={}, velocityZ={}", x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ);
    input_file.open(filename,std::ios::in|std::ios::out);
    std::streampos current = input_file.tellp();
    while (tmp_string.empty() or tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }
    input_file.seekp(current);
    int numParticles = std::stoi(tmp_string)+sizeX*sizeY*sizeZ;
    input_file << numParticles;
    input_file.close();

    double yClone = y;
    double zClone = z;
    double averageBrownianMotion = 0.1;
    input_file.open(filename,std::ios::in|std::ios::out|std::ios::app);
    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t j = 0; j < sizeY; j++)
        {
            for (size_t k = 0; k < sizeZ; k++) {
                std::array<double, 3> brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrownianMotion, 2);
                input_file << x << " " << y << " " << z << " " << velocityX + brownianMotion[0] << " " << velocityY + brownianMotion[1] << " " << velocityZ + brownianMotion[2] << " " << mass << "\n";         
                z += distance;
            }
            z = zClone;
            y += distance;
        }
        y = yClone;
        x += distance;
    }
    input_file.close();
}

void ParticleGenerator::generateDisc(double x, double y, double z, int MoleculesPerRadius, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char* filename) {
    spdlog::info("Generating disc with parameters: x={}, y={}, z={}, MoleculesPerRadius={}, distance={}, mass={}, velocityX={}, velocityY={}, velocityZ={}", x, y, z, MoleculesPerRadius, distance, mass, velocityX, velocityY, velocityZ);

    double radius = MoleculesPerRadius * distance;
    
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
    int numParticles = std::stoi(tmp_string) + static_cast<int>(M_PI * radius * radius / (distance * distance));
    input_file << numParticles;
    input_file.close();

    input_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!input_file.is_open()) {
        spdlog::error("Failed to open file {}", filename);
        return;
    }
    // Generate particles within the specified radius
    for (int i = -MoleculesPerRadius; i < MoleculesPerRadius; ++i) {
        for (int j = -MoleculesPerRadius; j < MoleculesPerRadius; ++j) {
            double size_x = i * distance;
            double size_y = j * distance;
            double distanceFromCenter = std::sqrt(size_x * size_x + size_y * size_y);

            if (distanceFromCenter <= radius) {
                double particleX = x + size_x;
                double particleY = y + size_y;
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
