#pragma once

#include <array>
#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

    /**
     * Used to create a density and velocity profile along the x0-axis of the domain of the simulation
     * */
    class ProfilingComponent {

    private:
        // size_t bins;
        // std::vector<double> densities;
        // std::vector<double> velocities;
        static bool ends_with(const std::string& str, const std::string& suffix);
    
    public:

    /**
     * @brief Constructor for ProfilingComponent.
     * 
     * Initializes a new instance of the ProfilingComponent class with default settings.
     */
    ProfilingComponent();

    /**
     * @brief Destructor for ProfilingComponent.
     * 
     * Cleans up resources used by the ProfilingComponent instance.
     */
    ~ProfilingComponent();

        /**
         * @brief Performs the analysis on the given data in pc.
         * @param bins Bin count
         * @param domainSize Simulation Domain Size
         * @param pc Particle Container
         * @param fileName Destination file name
         * */
        static void profile(size_t bins, std::array<double,3> domainSize, ParticleContainer& pc, const std::string &fileName);
    };