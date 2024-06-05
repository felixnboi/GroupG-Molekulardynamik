#pragma once

#include <string>

#include "XMLFormat.h" 
#include "../../data/Cuboid.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <vector>

class XMLReader{
    public:

        XMLReader();

        ~XMLReader();

        void readCuboids(const char* filename, std::vector<Cuboid>& cuboids);

        void readSimulation(const char* filename, std::string& baseName, unsigned& writeFrequency, double& start_time, double& end_time, double& delta_t);
};