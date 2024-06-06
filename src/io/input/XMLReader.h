#pragma once

#include <string>

#include "XMLFormat.h" 
#include "../../data/Cuboid.h"
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
         * @brief Reads cuboid data from an XML file and populates a vector of Cuboid objects.
         * @param filename The name of the XML file to read from.
         * @param cuboids A reference to a vector where the cuboid data will be stored.
         */
        void readCuboids(const char* filename, std::vector<Cuboid>& cuboids);
        /**
         * @brief Reads simulation parameters from an XML file.
         * @param filename The name of the XML file to read from.
         * @param baseName A reference to a string where the base name for output files will be stored.
         * @param writeFrequency A reference to an unsigned integer where the write frequency will be stored.
         * @param start_time A reference to a double where the start time of the simulation will be stored.
         * @param end_time A reference to a double where the end time of the simulation will be stored.
         * @param delta_t A reference to a double where the time step of the simulation will be stored.
         */
        void readSimulation(const char* filename, std::string& baseName, unsigned& writeFrequency, double& start_time, double& end_time, double& delta_t);
};