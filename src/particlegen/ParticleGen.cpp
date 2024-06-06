/**
 * @file ParticleGen.cpp
 */
#include "ParticleGenerator.h"
#include "../utils/NumericalUtils.h"
#include "../io/input/XMLReader.h"


/**
 * @brief Main function to generate particles using various input options.
 * @param argc The number of command-line arguments.
 * @param argsv The command-line arguments.
 * @return An integer representing the exit status of the application.
 */
int main(int argc, char *argsv[]){
    spdlog::set_level(spdlog::level::info);

    const char* const short_ops = "sx:y:z:m:d:";
    const option long_opts[] = {
        {"xml", required_argument, nullptr, 'p'},
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

    bool xml_flag = false;
    bool cli_flag = false;

    bool save_flag = false;
    bool xpos_flag = false;
    bool ypos_flag = false;
    bool zpos_flag = false;
    bool xsize_flag = false;
    bool ysize_flag = false;
    bool zsize_flag = false;
    bool xvel_flag = false;
    bool yvel_flag = false;
    bool zvel_flag = false;
    bool dist_flag = false;
    bool mass_flag = false;

    std::vector<Cuboid> cuboids;
    std::vector<Disc> discs;

    const char* xml_file = "";

    std::array<double, 3> position{};
    std::array<double, 3> velocity{}; 
    std::array<unsigned, 3> dimensions{}; 

    double distance = 0;
    double mass = 0;

    while((opt = getopt_long(argc, argsv, short_ops, long_opts, nullptr)) != -1){
        switch(opt){
            case 'p':{
                xml_file = optarg;
                xml_flag = true;
                break;
            }
            case 'l':{
                cli_flag = true;
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
                cli_flag = true;
                xvel_flag = true;
                if(isDouble(optarg)){
                    velocity[0] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityX");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'b':{
                cli_flag = true;
                yvel_flag = true;
                if(isDouble(optarg)){
                    velocity[1] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityY");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'c':{
                cli_flag = true;
                zvel_flag = true;
                if(isDouble(optarg)){
                    velocity[2] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for velocityZ");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'x':{
                cli_flag = true;
                xpos_flag = true;
                if(isDouble(optarg)){
                    position[0] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for x");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'y':{
                cli_flag = true;
                ypos_flag = true;
                if(isDouble(optarg)){
                    position[1] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for y");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'z':{
                cli_flag = true;
                zpos_flag = true;
                if(isDouble(optarg)){
                    position[2] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for z");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'X':{
                cli_flag = true;
                xsize_flag = true;
                if(isInteger(optarg)){
                    dimensions[0] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeX");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'Y':{
                cli_flag = true;
                ysize_flag = true;
                if(isInteger(optarg)){
                    dimensions[1] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeY");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'Z':{
                cli_flag = true;
                zsize_flag = true;
                if(isInteger(optarg)){
                    dimensions[2] = std::atof(optarg);
                    break;
                }else{
                    spdlog::error("Invalid argument for sizeZ");
                    ParticleGenerator::logHelp();
                    return EXIT_FAILURE;
                }
            }
            case 'd':{
                cli_flag = true;
                dist_flag = true;
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
                cli_flag = true;
                mass_flag = true;
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
                cli_flag = true;
                save_flag = true;
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

    if(xml_flag){
        inputFileManager::resetFile("../input/generated-input.txt");
        if(cli_flag){
            spdlog::error("Please use either xml or cli.");
            ParticleGenerator::logHelp();
            return EXIT_FAILURE;
        }
        XMLReader xmlreader;
        xmlreader.readCuboids(xml_file, cuboids);
        xmlreader.readDiscs(xml_file, discs);

    }else{
        if(!cli_flag){
            spdlog::error("Fail! Required command line arguments are missing.");
            ParticleGenerator::logHelp();
            return EXIT_FAILURE;
        }
        if(!save_flag){
        spdlog::warn("No '-s' flag provided. File was overwritten.");
        inputFileManager::resetFile("../input/generated-input.txt");
        }

        if(!(xvel_flag&&yvel_flag&&zvel_flag&&xpos_flag&&ypos_flag&&zpos_flag&&xsize_flag&&ysize_flag&&zsize_flag&&dist_flag&&mass_flag)){
            spdlog::error("Fail! Required command line arguments are missing.");
            ParticleGenerator::logHelp();
            return EXIT_FAILURE;
        }

        if(xvel_flag&&yvel_flag&&zvel_flag&&xpos_flag&&ypos_flag&&zpos_flag&&xsize_flag&&ysize_flag&&zsize_flag&&dist_flag&&mass_flag){
            cuboids.push_back(Cuboid(position, velocity, dimensions, distance, mass));
        }
        
    }


    for(const auto& cuboid : cuboids){
        ParticleGenerator::generateCuboid(cuboid, "../input/generated-input.txt");
    }

    for(const auto& disc : discs){
        ParticleGenerator::generateDisc(disc, "../input/generated-input.txt");
    }
    spdlog::info("ParticleGenerator application finished successfully.");
}