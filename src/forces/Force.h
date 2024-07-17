#pragma once

#include "Force.h"
#include "../particlecontainers/ParticleContainerLinkedCell.h"
#include <math.h>


/**
 * @brief This class is used to model the forces between particles in a system.
 */
class Force{

public:
    /**
     * @brief Constructor with all parameters.
     * 
     * @param reflectLenJonesFlag Array of booleans for reflection flags of the 6 bounderies.
     * @param periodicFlag Array of booleans for periodic boundary conditions in 3 dimensions.
     * @param lenJonesFlag Flag if the Lenard-Jones-Force should be calculated.
     * @param gravFlag Flag if the gravitational force should be calculated.
     * @param linkedcells The flag for choosing the algorithm. If set linkedcells algorithm is used.
     * @param gravConstant The gravitational constant value used for calculations.
     * @param membraneFlag The flag for the membrane simulation.
     * @param k The stiffness constant of the harmonic force.
     * @param r0 The average bond lenght of particles in a membrane.
     * @param strategy The value for determining the parallelization strategy to use.
     */
    Force(std::array<bool,6> reflectLenJonesFlag, std::array<bool,3> periodicFlag, bool lenJonesFlag, bool gravFlag, bool linkedcells, 
         std::array<double, 3> gravConstant, bool membraneFlag, double k, double r0, size_t startegy);

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
    void calculateFLennardJones(std::vector<std::pair<Particle* const, Particle* const>> pairs);

    /**
     * @brief Helper method for parallelization strategy.
     * 
     * @param pair The current pair of particle pointers to be used for calculations.
     */
    inline void calculateFLennardJonesHelper(const std::pair<Particle* const, Particle* const> &pair);

    /**
     * @brief Calculates the forces between all pairs of particles using the gravitational force.
     * 
     * @param pairs Vector of arrays containing pairs of particles.
     */
    void calculateFGravitation(std::vector<std::pair<Particle* const, Particle* const>> pairs);

    /**
     * @brief Calculates the Lennard Jones force between all pairs of particles which are connected through a peridic boundery.
     * 
     * @param LCContainer The Container of the particles.
     */
    void calculateFPeriodic(ParticleContainerLinkedCell &LCContainer);

    /**
     * @brief Helper method for parallelization strategy.
     * 
     * @param pair The current pair of particle pointers we want to use for calculations.
     * @param i Either 0 or 1. If 1 current particle pair is connected through the periodic boundary in x-direction.
     * @param j Either 0 or 1. If 1 current particle pair is connected through the periodic boundary in y-direction.
     * @param k Either 0 or 1. If 1 current particle pair is connected through the periodic boundary in z-direction.
     * @param LCContainer The linked cell particle container.
     */
    inline void calculateFPeriodicHelper(const std::pair<Particle* const, Particle* const> &pair, int i, int j, int k, ParticleContainerLinkedCell &LCContainer);

    /**
     * @brief Calculates the force for all particles which are to close to a reflecting boundery.
     * 
     * @param LCContainer The Container of the particles.
     */
    void calculateFReflecting(ParticleContainerLinkedCell &LCContainer);

    /**
     * @brief Helper method for parallelization strategy
     * 
     * @param particle The current pointer to use for calculation. 
     */
    inline void calculateFReflectingHelper(Particle* particle, ParticleContainerLinkedCell &LCContainer);

    /**
     * @brief Calculates the Lennard-Jones force between two particles.
     * 
     * @param direction Direction vector between two particles.
     * @param epsilon Depth of the potential well.
     * @param sigma Finite distance at which the inter-particle potential is zero.
     * @param cutOffRadiusSquared Cut-off radius beyond which the force is considered zero.
     * @return std::array<double, 3> Calculated force vector.
     */
    std::array<double,3> calculateLennardJonesForce(std::array<double,3> direction, double epsilon, double sigma, double cutOffRadiusSquared);

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
    void calculateHarmonicFroce(Particle* particle1, Particle* particle2, double k, double r0);

private:
    const std::array<bool,6> reflectLenJonesFlag; ///< If the corresponding boundery is reflecting.
    const std::array<bool,3> periodicFlag; ///< If the corresponding bounderies are periodic.
    const bool lenJonesFlag; ///< Flag if the Lenard-Jones-Force should be calculated.
    const bool gravFlag; ///< Flag if the the gravitational force should be calculated.
    const bool linkedcells; ///< The flag for choosing the algorithm. If set linkedcells algorithm is used.
    const std::array<double, 3> gravConstant; ///< The gravitational constant value used for calculations.
    const bool membraneFlag; ///< Flag for the membrane simulation.
    const double k; ///< The stiffness constant of the harmonic force.
    const double r0; ///< The average bond lenght of particles in a membrane.
    const size_t strategy; ///< The value to determine which parallelization strategy to use.
    const double twoRoot6; ///< Sixth root of two.
};
