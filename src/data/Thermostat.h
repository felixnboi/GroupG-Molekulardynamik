#pragma once

#include "../ParticleContainers/ParticleContainer.h"
#include "../data/Particle.h"
#include "../data/Cuboid.h"
#include "../data/Disc.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../inputFileManager.h"
#include "../utils/ArrayUtils.h"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <string>

/**
 * @brief A thermostat that can be used to control the temperature of a particle container. Allows for gradual scaling of the temperature
 * towards a set target.
 */
class Thermostat {
    private:
    /**The number of iterations between thermostat applications.*/
    size_t nThermostat;
    /**Thermostat needs to know the number of dimenions for the temperature-computation*/
    size_t dimensions;
    /**Target temperature that the Thermostat tries to reach*/
    double targetTemp;
    /**determines the maximum temperature manipulation in one iteration step*/
    double maxDeltaTemp;


public:

    /**
     * @brief Default constructor.
     */
    Thermostat();
    /**
     * @brief Constructor for Thermostat.
     * 
     * @param targetTemp The target temperature for thermostat applications.
     * @param maxDeltaTemp The maximum temperature change allowed per thermostat application.
     * @param nThermostat The step size of the thermostat.
     * @param dimensions The number of dimensons of the simulation.
     */
    Thermostat(double targetTemp, double maxDeltaTemp = std::numeric_limits<double>::max(),
               size_t nThermostat = 1000, size_t dimensions=2);

    /**
     * @brief Destructor for Thermostat.
     */
    ~Thermostat() = default;

    /**
     * @brief Get the current temperature of a particle container.
     *
     * @param pc The particle container to get the temperature of.
     * @return double The current temperature of the particle container.
     */
    double getCurrentTemperature(const std::unique_ptr<ParticleContainer>& pc) const;

    /**
     * @brief Scale the temperature of a particle container towards the target temperature. Capped by the maximum temperature change.
     *
     * @param pc The particle container to scale the temperature of.
     */
    void scaleWithBeta(std::unique_ptr<ParticleContainer>& pc);

    /**
     * @brief Set the initial temperature of a particle container. This function sets the velocity of all particles in the container to a
     * random value according to the Maxwell-Boltzmann distribution (all previous velocities are discarded).
     * Use this function for systems with no initial velocity.
     *
     * @param new_initialTemp The initial temperature used to set the initial velocity of the particles.
     * @param pc The particle container to set the initial temperature of.
     */
    void initSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc);
    
    /**
     * @brief Get the target temperature of the thermostat.
     *
     * @return double The target temperature of the thermostat.
     */
    double getTargetTemp() const;

    /**
     * @brief Get the maximum temperature change of the thermostat.
     *
     * @return double The maximum temperature change of the thermostat.
     */
    double getMaxDeltaTemp() const;

    /**
     * @brief Get the number of iterations between thermostat applications.
     *
     * @return size_t The number of iterations between thermostat applications.
     */
    size_t getNThermostat() const;

    /**
     * @brief Get the number of dimensions of expected to be used.
     *
     * @return size_t The number of dimensions of expected to be used.
     */
    size_t getDimensions() const;
};