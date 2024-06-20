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
     * @param reflectionLenJonesFlag The flags for our 6 boundaries in 3d space. 
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     */
    void calculateF(ParticleContainer &particles, bool linkedcells, double gravConstant) override;

    void calculateFPairs(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs);

    std::array<double,3> calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadius);
};
