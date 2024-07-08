#pragma once

#include "Force.h"
#include "../particlecontainers/ParticleContainerLinkedCell.h"
#include <math.h>


/**
 * @brief This class is used to model the forces between particles in a system.
 */
class Force
{
private:
    std::array<bool,6> reflectLenJonesFlag; ///< If the corresponding boundery is reflecting.
    std::array<bool,3> periodicFlag; ///< If the corresponding bounderies are periodic.
    bool lenJonesFlag; ///< Flag if the Lenard-Jones-Force should be calculated.
    bool gravFlag; ///< Flag if the the gravitational force should be calculated.
    bool harmonicFlag; ///< Flag if the harmonic force should be calculated.
    bool linkedcells; ///< The flag for choosing the algorithm. If set linkedcells algorithm is used.
    double gravConstant; ///< The gravitational constant value used for calculations.
    double k; ///< The stiffness constant of the harmonic force.
    double r0; ///< The average bond lenght of particles in a membrane.

public:

    /**
     * @brief Default constructor for Force.
     * 
     * @param reflectLenJonesFlag Array of booleans for reflection flags of the 6 bounderies.
     * @param periodicFlag Array of booleans for periodic boundary conditions in 3 dimensions.
     * @param lenJonesFlag Flag if the Lenard-Jones-Force should be calculated.
     * @param gravFlag Flag if the gravitational force should be calculated.
     * @param harmonicFlag Flag if the harmonic force should be calculated.
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     * @param gravConstant The gravitational constant value used for calculations.
     * @param k The stiffness constant of the harmonic force.
     * @param r0 The average bond lenght of particles in a membrane.
     */
    Force(std::array<bool,6> reflectLenJonesFlag, std::array<bool,3> periodicFlag, bool lenJonesFlag, bool gravFlag, bool harmonicFlag, bool linkedcells, 
         double gravConstant, double k, double r0);

    /**
     * @brief Default destructor for Force.
     */
    ~Force();

    /**
     * @brief Calculates the forces between particles in a container.
     * 
     * This function calculates the forces between all pairs of particles in the given container.
     * It updates the force vectors of the particles accordingly.
     * 
     * @param particles The container of particles for which to calculate the forces.
     */
    void calculateF(ParticleContainer &particles);

    /**
    * @brief Calculates the forces between all pairs of particles using the Lennard-Jones potential and ignoring peridic bounderies.
    * 
    * @param pairs Vector of arrays containing pairs of particles.
    */
    void calculateFLennardJones(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs);

    /**
    * @brief Calculates the forces between all pairs of particles using the gravitational force.
    * 
    * @param pairs Vector of arrays containing pairs of particles.
    */
    void calculateFGravitation(std::vector<std::array<std::shared_ptr<Particle>, 2UL>> pairs);

    /**
     * @brief Calculates the Lennard Jones force between all pairs of particles which are connected through a peridic boundery.
    * 
    * @param LCContainer The Container of the particles.
     */
    void calculateFPeriodic(ParticleContainerLinkedCell &LCContainer);

    /**
     * @brief Calculates the force for all particles which are to close to a reflecting boundery.
    * 
    * @param LCContainer The Container of the particles.
     */
    void calculateFReflecting(ParticleContainerLinkedCell &LCContainer);

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

    /**
     * @brief Calculates the harmonic force for all particles.
    * 
    * @param LCContainer The Container of the particles.
    * @param k The stiffness constant.
    * @param r0 The average bond lenght.
     */
    void calculateFHarmonic(ParticleContainerLinkedCell &LCContainer, double k, double r0);

    /**
     * @brief Calculates the harmonic force between two particles.
    * 
    * @param LCContainer The Container of the particles.
    * @param k The stiffness constant.
    * @param r0 The average bond lenght.
    * @param particle1 The first of the two particles.
    * @param particle2 The second of the two particles.
     */
    void calculateHarmonicFroce(std::shared_ptr<Particle> particle1, std::shared_ptr<Particle> particle2, double k, double r0);

};
