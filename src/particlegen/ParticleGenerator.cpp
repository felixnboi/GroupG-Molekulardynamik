#include "ParticleGenerator.h"

void ParticleGenerator::generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ, const char *filename) {
    std::fstream input_file;
    std::string tmp_string;
    
    spdlog::info("Generating cuboid with parameters: x={}, y={}, z={}, sizeX={}, sizeY={}, sizeZ={}, distance={}, mass={}, velocityX={}, velocityY={}, velocityZ={}", x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ);
    input_file.open(filename,std::ios::in|std::ios::out);
    std::streampos curent = input_file.tellp();
    while (tmp_string.empty() or tmp_string[0] == '#') {
        curent = input_file.tellp();
        getline(input_file, tmp_string);
    }
    input_file.seekp(curent);
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

void ParticleGenerator::logHelp(){
    spdlog::info("Usage: \"./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double\"");
    spdlog::info("For further information please read the README.md file at top level.");
    spdlog::info("Terminating...");
}
