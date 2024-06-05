#pragma once

#include <string>

#include "XMLFormat.h" 
#include "../../data/Cuboid.h"
#include "../../data/Disc.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <vector>

class XMLReader{
    public:

        XMLReader();

        ~XMLReader();

        void readCuboids(const char* filename, std::vector<Cuboid>& cuboids);

        void readDiscs(const char* filename, std::vector<Disc>& discs);

        void readSimulation(const char* filename, bool& generate_flag, std::string& inputFile, std::string& baseName, 
        unsigned& writeFrequency, double& start_time, double& end_time, double& delta_t, std::string& force, std::string& algorithm, 
        std::string& loglevel, std::array<std::string, 6>& boundary, double& cutoff_radius, std::array<double, 3>& domain);
};