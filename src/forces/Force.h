#pragma once
#include "../utils/ArrayUtils.h"
#include "../particlecontainers/ParticleContainer.h"
#include "spdlog/spdlog.h"

/**
 * @brief This abstract class serves as an interface for calculating forces acting on particles.
 * Subclasses must implement the calculateF() method to define the specific force calculation logic.
 */
class Force
{
public:
    /**
     * @brief Virtual destructor of class Force
     */
    virtual ~Force() = default;

    /**
     * @brief Calculates the forces acting on particles.
     * @param particles The container of particles for which to calculate the forces.
     * @param reflectionLenJonesFlag The flags for our 6 boundaries in 3d space. 
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     */
    virtual void calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) = 0;
};