#include "ParticleGenerator.h"
#include "spdlog/spdlog.h"


bool ParticleGenerator::isDouble(char* str) {
    try {
        std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool ParticleGenerator::isInteger(char* str) {
    try {
        std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}


void ParticleGenerator::generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ) {
    std::fstream input_file;
    std::string tmp_string;
    
    spdlog::info("Generating cuboid with parameters: x={}, y={}, z={}, sizeX={}, sizeY={}, sizeZ={}, distance={}, mass={}, velocityX={}, velocityY={}, velocityZ={}", x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ);
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
    double averageBrownianMotion = 0.1;
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out|std::ios::app);
    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t j = 0; j < sizeY; j++)
        {
            for (size_t k = 0; k < sizeZ; k++) {
                std::array<double, 3> brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrownianMotion, 3);
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
    spdlog::info("Starting ParticleGenerator application");

    const char* const short_ops = "sx:y:z:m:d:";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"sizeX", required_argument, nullptr, 'X'},
        {"sizeY", required_argument, nullptr, 'Y'},
        {"sizeZ", required_argument, nullptr, 'Z'},
        {"velocityX", required_argument, nullptr, 'a'},
        {"velocityY", required_argument, nullptr, 'b'},
        {"velocityZ", required_argument, nullptr, 'c'},
        {nullptr, no_argument, nullptr, 0}
    };
    int opt;

    bool s_flag = false;
    bool x_flag = false;
    bool y_flag = false;
    bool z_flag = false;
    bool X_flag = false;
    bool Y_flag = false;
    bool Z_flag = false;
    bool a_flag = false;
    bool b_flag = false;
    bool c_flag = false;
    bool d_flag = false;
    bool m_flag = false;

    double x = 0; double y = 0; double z = 0; double distance = 0; double mass = 0; double velocityX = 0; double velocityY = 0; double velocityZ = 0;
    size_t sizeX = 0; size_t sizeY = 0; size_t sizeZ = 0;

    while((opt = getopt_long(argc, argsv, short_ops, long_opts, nullptr)) != -1){
        switch(opt){
            case 'a':{
                a_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    velocityX = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityX");
                    return EXIT_FAILURE;
                }
            }
            case 'b':{
                b_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    velocityY = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityY");
                    return EXIT_FAILURE;
                }
            }
            case 'c':{
                c_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    velocityZ = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityZ");
                    return EXIT_FAILURE;
                }
            }
            case 'x':{
                x_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    x = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for x");
                    return EXIT_FAILURE;
                }
            }
            case 'y':{
                y_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    y = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for y");
                    return EXIT_FAILURE;
                }
            }
            case 'z':{
                z_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    z = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for z");
                    return EXIT_FAILURE;
                }
            }
            case 'X':{
                X_flag = true;
                if(ParticleGenerator::isInteger(optarg)){
                    sizeX = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeX");
                    return EXIT_FAILURE;
                }
            }
            case 'Y':{
                Y_flag = true;
                if(ParticleGenerator::isInteger(optarg)){
                    sizeY = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeY");
                    return EXIT_FAILURE;
                }
            }
            case 'Z':{
                Z_flag = true;
                if(ParticleGenerator::isInteger(optarg)){
                    sizeZ = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeZ");
                    return EXIT_FAILURE;
                }
            }
            case 'd':{
                d_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    distance = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for distance");
                    return EXIT_FAILURE;
                }
            }
            case 'm':{
                m_flag = true;
                if(ParticleGenerator::isDouble(optarg)){
                    mass = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for mass");
                    return EXIT_FAILURE;
                }
            }
            case 's':{
                s_flag = true;
                break;
            }
            case 'h':{
                spdlog::info("Help option selected");
                return EXIT_SUCCESS;
            }
            case '?':{
                spdlog::error("Invalid option");
                return EXIT_FAILURE;
            }
        }
    }
    if(!s_flag){
        spdlog::warn("No '-s' flag provided");
        inputFileManager::resetFile();
    }

    if(!(a_flag&&b_flag&&c_flag&&x_flag&&y_flag&&z_flag&&X_flag&&Y_flag&&Z_flag&&d_flag&&m_flag)){
        spdlog::error("Fail! Required arguments are missing");
        return EXIT_FAILURE;
    }

    ParticleGenerator::generateCuboid(x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ);
    spdlog::info("ParticleGenerator application finished successfully");
}


