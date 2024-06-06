#pragma once

#include <string>

#include "XMLFormat.h" 
#include "../../data/Cuboid.h"
#include "../../data/Disc.h"
#include "../../data/SimData.h"
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

        void readSimulation(const char* filename, SimData& simdata);
};