#include "ParticleGenerator.h"
#include "spdlog/spdlog.h"


void ParticleGenerator::generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ) {
    spdlog::info("Generating cuboid of particles");
    spdlog::debug("Initial position: ({}, {}, {}), Cuboid dimensions: {}x{}x{}, Particle distance: {}", x, y, z, sizeX, sizeY, sizeZ, distance);
    spdlog::trace("Particle mass: {}, Initial velocity: ({}, {}, {})", mass, velocityX, velocityY, velocityZ);

    std::fstream input_file;
    std::string tmp_string;
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out);
    while (tmp_string.empty() or tmp_string[0] == '#') {
        getline(input_file, tmp_string);
    }
    input_file.seekp(input_file.tellp()-33);
    int numParticles = std::stoi(tmp_string)+sizeX*sizeY*sizeZ;
    input_file << numParticles;
    input_file.close();
    double yClone = y;
    double zClone = z;
    double averageBrwoinanMotion = 0.1;
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out|std::ios::app);
    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t j = 0; j < sizeY; j++)
        {
            for (size_t k = 0; k < sizeZ; k++) {
                std::array<double, 3> brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrwoinanMotion, 3);
                input_file << x << " " << y << " " << z << " " << velocityX + brownianMotion[0] << " " << velocityY + brownianMotion[1] << " " << velocityZ + brownianMotion[2] << " " << mass << "\n";                z += distance;
            }
            z = zClone;
            y += distance;
        }
        y = yClone;
        x += distance;
    }
    input_file.close();
}

int main(int argc, char *argsv[]){
    int opt;

    bool s_flag = false;

    while((opt = getopt(argc, argsv, "s")) != 1){
        switch(opt){
            case 's':
                s_flag = true;
                break;
            case '?':
                spdlog::error("Unknown option passed: {}", optopt);
                return EXIT_FAILURE;
        }
    }

    if(!s_flag){
        spdlog::info("Resetting generated input file");
        //reset generated input file
    }
}

