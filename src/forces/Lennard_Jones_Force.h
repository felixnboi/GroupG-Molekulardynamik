#pragma once

#include "Force.h"
#include "../particlecontainers/ParticleContainerLinkedCell.h"
#include <math.h>


/**
 * @brief The Lenard-Jones force is used to model the interaction between particles in a system.
 */
class Lennard_Jones_Force : public Force
{
private:
    std::array<bool,6> reflectLenJonesFlag;
    std::array<bool,3> periodicFlag;
public:

    /**
     * @brief Default constructor for Lenard_Jones_Force.
     * 
     * @param reflectLenJonesFlag Array of booleans for reflection flags in 6 directions.
     * @param periodicFlag Array of booleans for periodic boundary conditions in 3 dimensions.
     */
    Lennard_Jones_Force(std::array<bool,6> reflectLenJonesFlag, std::array<bool,3> periodicFlag);

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
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     * @param gravConstant The gravitational constant value used for calculations.
     */
    void calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) override;

    /**
    * @brief Calculates the forces between pairs of particles using the Lennard-Jones potential.
    * 
    * @param pairs Vector of arrays containing pairs of particles.
    */
    void calculateFPairs(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs);

    /**
    * @brief Calculates the Lennard-Jones force between two particles.
    * 
    * @param direction Direction vector between two particles.
    * @param epsilon Depth of the potential well.
    * @param sigma Finite distance at which the inter-particle potential is zero.
    * @param cutOffRadius Cut-off radius beyond which the force is considered zero.
    * @return std::array<double, 3> Calculated force vector.
    */
    std::array<double,3> calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadius);
};
