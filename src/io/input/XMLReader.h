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
/**
 * @brief The XMLReader class is responsible for reading simulation and cuboid data from XML files.
 */
class XMLReader{
    public:
    
        /**
         * @brief Constructor for the XMLReader class.
         */
        XMLReader();

        /**
         * @brief Destructor for the XMLReader class.
         */
        ~XMLReader();

        /**
         * @brief Reads cuboid data from an XML file and populates a vector of cuboid objects.
         * @param filename The name of the XML file to read from.
         * @param cuboids A reference to a vector where the cuboid data will be stored.
         */
        void readCuboids(const char* filename, std::vector<Cuboid>& cuboids);


        /**
         * @brief Reads cuboid data from an XML file and populates a vector of disc objects.
         * @param filename The name of the XML file to read from.
         * @param discs A reference to a vector where the disc data will be stored.
         */
        void readDiscs(const char* filename, std::vector<Disc>& discs);

        /**
         * @brief Reads simulation data from an XML file and saves it in a simdata object.
         * @param filename The name of the XML file to read from.
         * @param simdata A reference to the simdata object.
         */
        void readSimulation(const char* filename, SimData& simdata);
};