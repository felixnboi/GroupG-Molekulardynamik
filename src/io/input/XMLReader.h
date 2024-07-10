#pragma once

#include "XMLFormat.h" 
#include "../../data/Cuboid.h"
#include "../../data/Disc.h"
#include "../../data/SimData.h"
#include "../../data/ThermostatData.h"
#include "../../data/CheckpointData.h"
#include "../../data/MembraneData.h"
#include "spdlog/spdlog.h"

#include <string>
#include <memory>
#include <string>
#include <vector>
#include <limits.h>

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
     * 
     * @param filename The name of the XML file to read from.
     * @param cuboids A reference to a vector where the cuboid data will be stored.
     */
    void readCuboids(const char* filename, std::vector<Cuboid>& cuboids, double epsilon, double sigma);


    /**
     * @brief Reads cuboid data from an XML file and populates a vector of disc objects.
     * 
     * @param filename The name of the XML file to read from.
     * @param discs A reference to a vector where the disc data will be stored.
     */
    void readDiscs(const char* filename, std::vector<Disc>& discs, double epsilon, double sigma);

    /**
     * @brief Reads simulation data from an XML file and saves it in a simdata object.
     * 
     * @param filename The name of the XML file to read from.
     * @param simdata A reference to the simdata object.
     */
    void readSimulation(const char* filename, SimData& simdata);

    /**
     * @brief Reads thermostat data from an XML file and saves it in a thermostatdata object.
     * 
     * @param filename The name of the XML file to read from.
     * @param thermostatdata A reference to the thermostatdata object.
     */
    void readThermostat(const char* filename, ThermostatData& thermostatdata);

    /**
     * @brief Reads checkpoint data from an XML file and saves it in a checkpointdata object.
     * 
     * @param filename The name of the XML file to read from.
     * @param checkpointdata A reference to the checkpointdata object.
     */
    void readCheckpoint(const char* filename, CheckpointData& checkpointdata);

    /**
     * @brief Reads membrane data from an XML file and saves it in a membranedata object.
     * 
     * @param filename The name of the XML file to read from.
     * @param membranedata A reference to the membranedata object.
     */
    void readMembrane(const char* filename, MembraneData& membranedata);
};