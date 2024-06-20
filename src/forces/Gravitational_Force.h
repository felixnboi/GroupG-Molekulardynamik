#pragma once

#include "Force.h"

#include <math.h>

/**
 * @brief This class implements the Force interface to calculate gravitational forces between particles
 * based on their masses and positions.
 */
class Gravitational_Force : public Force
{
public:

    /**
     * @brief Default constructor.
     */
    Gravitational_Force();

    /**
     * @brief Destructor.
     */
    ~Gravitational_Force();

    /**
     * @brief Calculates gravitational forces between particles.
     * 
     * This method calculates gravitational forces between particles based on their masses and positions.
     * It iterates over all pairs of particles and updates their force vectors accordingly.
     * 
     * @param particles The container of particles for which to calculate the forces.
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     * @param gravConstant The gravitational constant value used for calculations.
     */
    void calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) override;
};