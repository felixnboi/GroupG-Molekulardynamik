#pragma once
#include "../ParticleContainer.h"

/**
 * @brief This abstract class serves as an interface for calculating forces acting on particles.
 * Subclasses must implement the calculateF() method to define the specific force calculation logic.
 */
class Force
{
public:
    /**
     * @brief Calculates the forces acting on particles.
     * @param particles The ParticleContainer containing the particles on which forces are to be calculated.
     */
    virtual void calculateF(ParticleContainer &particles){};
    virtual ~Force() {}
};