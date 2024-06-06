#pragma once

#include "Force.h"
#include "../ParticleContainers/ParticleContainerLinkedCell.h"
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
     * @param reflectionLenJonesFlag The flags for our 6 boundaries in 3d space. 
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     * @param epsilon A constant important for calculating Lennardjones-Force.
     * @param sigma A constant important for calculating Lennardjones-Force.
     */
    void calculateF(ParticleContainer &particles, std::array<bool,6> reflectLenJonesFlag, bool linkedcells, 
    double epsilon, double sigma) override;
};
