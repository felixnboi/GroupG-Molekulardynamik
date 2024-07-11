#pragma once

#include <array>
#include <fstream>
#include "../../particlecontainers/ParticleContainerOld.h"
#include "../../particlecontainers/ParticleContainerLinkedCell.h"

/**
 * @brief Namespace used to create a density and velocity profile along the X-axis of the simulation´s domain.
 */
namespace ProfilingComponent {
    /**
     * @brief Checks whether the given string ends with the given suffix.
     * 
     * @param str The string to conssider.
     * @param suffix The suffix to be compared with.
     */
    bool ends_with(const std::string& str, const std::string& suffix);

    /**
     * @brief Performs the analysis on the given data in pc.
     * 
     * @param bins Bin count.
     * @param domainSize Simulation Domain Size.
     * @param pc Particle Container.
     * @param fileName Destination file name.
     */
    void profile(size_t bins, std::array<double,3> domainSize, ParticleContainer& pc, const std::string &fileName);
}