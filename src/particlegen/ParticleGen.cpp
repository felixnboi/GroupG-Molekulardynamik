#include "ParticleGenerator.h"
#include "../utils/NumericalUtils.h"


int main(int argc, char *argsv[]){
    spdlog::set_level(spdlog::level::info);

    const char* const short_ops = "sx:y:z:m:d:";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"log", required_argument, nullptr, 'l'},
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
            case 'l':{
            //parsing logging level
            if(std::string(optarg) == std::string("OFF")){
                spdlog::set_level(spdlog::level::off);
                spdlog::info("Logging level set to OFF");
                break;
            }
            if(std::string(optarg) == std::string("ERROR")){
                spdlog::set_level(spdlog::level::err);
                spdlog::info("Logging level set to ERROR");
                break;
            }
            if(std::string(optarg) == std::string("WARN")){
                spdlog::set_level(spdlog::level::warn);
                spdlog::info("Logging level set to WARN");
                break;
            }
            if(std::string(optarg) == std::string("INFO")){
                spdlog::set_level(spdlog::level::info);
                spdlog::info("Logging level set to INFO");
                break;
            }
            if(std::string(optarg) == std::string("DEBUG")){
                spdlog::set_level(spdlog::level::debug);
                spdlog::info("Logging level set to DEBUG");
                break;
            }
            if(std::string(optarg) == std::string("TRACE")){
                spdlog::set_level(spdlog::level::trace);
                spdlog::info("Logging level set to TRACE");
                break;
            }
            spdlog::error("Invalid logging level: {}", optarg);
            ParticleGenerator::logHelp();
            return EXIT_FAILURE;
            }
            case 'a':{
                a_flag = true;
                if(isDouble(optarg)){
                    velocityX = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityX");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'b':{
                b_flag = true;
                if(isDouble(optarg)){
                    velocityY = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityY");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'c':{
                c_flag = true;
                if(isDouble(optarg)){
                    velocityZ = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityZ");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'x':{
                x_flag = true;
                if(isDouble(optarg)){
                    x = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for x");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'y':{
                y_flag = true;
                if(isDouble(optarg)){
                    y = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for y");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'z':{
                z_flag = true;
                if(isDouble(optarg)){
                    z = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for z");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'X':{
                X_flag = true;
                if(isInteger(optarg)){
                    sizeX = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeX");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'Y':{
                Y_flag = true;
                if(isInteger(optarg)){
                    sizeY = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeY");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'Z':{
                Z_flag = true;
                if(isInteger(optarg)){
                    sizeZ = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeZ");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'd':{
                d_flag = true;
                if(isDouble(optarg)){
                    distance = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for distance");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'm':{
                m_flag = true;
                if(isDouble(optarg)){
                    mass = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for mass");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 's':{
                s_flag = true;
                break;
            }
            case 'h':{
                spdlog::info("Help option selected");
                ParticleGenerator::logHelp();
                return EXIT_SUCCESS;
            }
            case '?':{
                spdlog::error("Invalid option");
                ParticleGenerator::logHelp();
                return EXIT_FAILURE;
            }
        }
    }

    spdlog::info("Starting ParticleGenerator application");

    if(!s_flag){
        spdlog::warn("No '-s' flag provided. File was overwritten.");
        inputFileManager::resetFile("../input/generated-input.txt");
    }

    if(!(a_flag&&b_flag&&c_flag&&x_flag&&y_flag&&z_flag&&X_flag&&Y_flag&&Z_flag&&d_flag&&m_flag)){
        spdlog::error("Fail! Required arguments are missing.");
        ParticleGenerator::logHelp();
        return EXIT_FAILURE;
    }

    ParticleGenerator::generateCuboid(x, y, z, sizeX, sizeY, sizeZ, distance, mass, velocityX, velocityY, velocityZ, "../input/generated-input.txt");
    spdlog::info("ParticleGenerator application finished successfully.");
}