#pragma once

#include "Force.h"

#include <math.h>


/**
 * @brief The Lenard-Jones force is used to model the interaction between particles in a system.
 */
class Lennard_Jones_Force : public Force
{
public:
    /**
     * @brief Default constructor for Lenard_Jones_Force.
     */
    Lennard_Jones_Force();
    /**
     * @brief Default destructor for Lenard_Jones_Force.
     */
    ~Lennard_Jones_Force();
    /**
     * @brief Calculates the Lenard-Jones forces between particles in a container.
     * 
     * This function calculates the Lenard-Jones forces between all pairs of particles in the given container.
     * It updates the force vectors of the particles accordingly.
     * 
     * @param particles The container of particles for which to calculate the forces.
     */
    void calculateF(ParticleContainer &particles) override;
};
