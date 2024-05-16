#include "ParticleGenerator.h"

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
        inputFileManager::resetFile("../input/generated-input.txt");
    }

    if(!(a_flag&&b_flag&&c_flag&&x_flag&&y_flag&&z_flag&&X_flag&&Y_flag&&Z_flag&&d_flag&&m_flag)){
        spdlog::error("Fail! Required arguments are missing");
        return EXIT_FAILURE;
    }

    ParticleGenerator::generateCuboid(x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ, "../input/generated-input.txt");
    spdlog::info("ParticleGenerator application finished successfully");
}
