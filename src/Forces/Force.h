#pragma once
#include "../utils/ArrayUtils.h"
#include "../ParticleContainers/ParticleContainer.h"
#include "spdlog/spdlog.h"

/**
 * @brief This abstract class serves as an interface for calculating forces acting on particles.
 * Subclasses must implement the calculateF() method to define the specific force calculation logic.
 */
class Force
{
public:

    virtual ~Force() = default;

    /**
     * @brief Calculates the forces acting on particles.
     * @param particles The ParticleContainerOld containing the particles on which forces are to be calculated.
     */
    virtual void calculateF(ParticleContainer &particles, std::array<bool,6> reflectLenJonesFlag, bool linkedcells, 
    double epsilon, double sigma) = 0;
};