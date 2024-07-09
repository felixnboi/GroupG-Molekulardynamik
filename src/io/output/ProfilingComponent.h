#pragma once

#include <array>
#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

    /**
     * Used to create a density and velocity profile along the X-axis of the simulation´s domain.
     * */
    namespace ProfilingComponent {

        static bool ends_with(const std::string& str, const std::string& suffix);

        /**
         * @brief Performs the analysis on the given data in pc.
         * @param bins Bin count.
         * @param domainSize Simulation Domain Size.
         * @param pc Particle Container.
         * @param fileName Destination file name.
         * */

        static void profile(size_t bins, std::array<double,3> domainSize, ParticleContainer& pc, const std::string &fileName);
    };