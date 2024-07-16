#include "ParticleGenerator.h"

void ParticleGenerator::generateCuboid(const Cuboid& cuboid, const char *filename) {
    std::fstream input_file;
    std::string tmp_string;

    std::array<double, 3> position = cuboid.getPosition();
    const std::array<double, 3> velocity = cuboid.getVelocity();
    const std::array<unsigned, 3> dimensions = cuboid.getDimensions();
    const double mass = cuboid.getMass();
    const double distance = cuboid.getDistance();
    const double epsilon = cuboid.getEpsilon();
    const double sigma = cuboid.getSigma();
    const size_t brownian_motion_dimension = cuboid.getBrownianMotionDimension();
    const size_t type = cuboid.getType();
    const bool is_outer = cuboid.getIsOuter();
    
    spdlog::info("Generating cuboid");
    
    input_file.open(filename,std::ios::in|std::ios::out);
    std::streampos current = input_file.tellp();

    while (tmp_string.empty() or tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }

    input_file.seekp(current);
    const int numParticles = std::stoi(tmp_string)+dimensions[0]*dimensions[1]*dimensions[2];
    input_file << numParticles;
    input_file.close();
    
    const double yClone = position[1];
    const double zClone = position[2];
    
    const double averageBrownianMotion = cuboid.getBrownianMotion();
    input_file.open(filename,std::ios::in|std::ios::out|std::ios::app);
    for (size_t i = 0; i < dimensions[0]; i++)
    {
        for (size_t j = 0; j < dimensions[1]; j++)
        {
            for (size_t k = 0; k < dimensions[2]; k++) {
                std::array<double, 3> brownianMotion = {0,0,0};
                if(brownian_motion_dimension != 0) brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrownianMotion, brownian_motion_dimension);
                input_file << position[0] << " " << position[1] << " " << position[2] << " " << velocity[0] + brownianMotion[0] << " " << velocity[1] + brownianMotion[1] << " " << velocity[2] + brownianMotion[2] << " " << mass << " " << epsilon << " "<< sigma << " "<< type << " "<< is_outer << "\n";         
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


void ParticleGenerator::generateDisc(const Disc& disc, const char* filename) {
    const std::array<double ,3> position = disc.getPosition();
    const std::array<double ,3> velocity = disc.getVelocity();
    const int MoleculesPerRadius = disc.getRadius();
    const double distance = disc.getDistance();
    const double mass = disc.getMass();
    const double epsilon = disc.getEpsilon();
    const double sigma = disc.getSigma();
    const size_t type = disc.getType();
    const bool is_outer = disc.getIsOuter();

    spdlog::info("Generating disc");

    std::fstream input_file;
    std::string tmp_string;
    const double radius = MoleculesPerRadius * distance;

    
    input_file.open(filename, std::ios::in | std::ios::out);
    if (!input_file.is_open()) {
        spdlog::error("Failed to open file {}", filename);
        return;
    }
    // skipping comments and getting the number of particles
    std::streampos current = input_file.tellp();
    while (tmp_string.empty() || tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }

    input_file.seekp(current);
    int numParticles = std::stoi(tmp_string);//+static_cast<int>(M_PI*radius*radius/(distance*distance));

    for (int i = -MoleculesPerRadius; i <= MoleculesPerRadius; ++i) {
        for (int j = -MoleculesPerRadius; j <= MoleculesPerRadius; ++j) {
            const double size_x = i * distance;
            const double size_y = j * distance;
            const double distanceFromCenter = std::sqrt(size_x * size_x + size_y * size_y);
            if (distanceFromCenter <= radius) {
                numParticles++;
            }
        }
    }

    input_file << numParticles;
    input_file.close();

    input_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!input_file.is_open()) {
        spdlog::error("Failed to open file {}", filename);
        return;
    }
    // Generate particles within the specified radius
    for (int i = -MoleculesPerRadius; i <= MoleculesPerRadius; ++i) {
        for (int j = -MoleculesPerRadius; j <= MoleculesPerRadius; ++j) {
            const double size_x = i * distance;
            const double size_y = j * distance;
            const double distanceFromCenter = std::sqrt(size_x * size_x + size_y * size_y);

            if (distanceFromCenter <= radius) {
                const double particleX = position[0] + size_x;
                const double particleY = position[1] + size_y;
                const double particleZ = position[2];

                input_file << particleX << " " << particleY << " " << particleZ << " "
                           << velocity[0] << " " << velocity[1] << " " << velocity[2] << " "
                           << mass << " " << epsilon << " " << sigma  <<" "<< type << " "<< is_outer << "\n";
            }
        }
    }
    input_file.close();
}

void ParticleGenerator::logHelp(){
    spdlog::info("Usage: \"[./ParticleGenerator --xml string [--help]] | [./ParticleGenerator [--help] [-s] -m double -d double -x double -y double -z double --sizeX int --sizeY int --sizeY int --velocityX double --velocityY double --velocityZ double]\"");
    spdlog::info("For further information please read the README.md file at top level.");
    spdlog::info("Terminating...");
}
