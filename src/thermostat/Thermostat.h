#pragma once

#include "../particlecontainers/ParticleContainer.h"
#include "../data/Particle.h"
#include "../data/Cuboid.h"
#include "../data/Disc.h"
#include "../data/ThermostatData.h"
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
public:
    /**
     * @brief Default constructor.
     */
    Thermostat();

    /**
     * @brief Constructor with the corresponding data.
     */
    Thermostat(ThermostatData thermostat_data);

    /**
     * @brief Destructor for Thermostat.
     */
    ~Thermostat() = default;

    /**
     * @brief Get the current temperature of a particle container.
     *
     * @param pc The particle container to get the temperature of.
     * @return The current temperature of the particle container.
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
     * @brief Getter for the data of the thermostat.
     * 
     * @return The data of the thermostat.
     */
    const ThermostatData& getThermostatData() const;

    /**
     * @brief Setter for the data of the thermostat.
     * 
     * @param new_thermostat_data The new gravitational constant used in simulation.
     */
    void setThermostatData(const ThermostatData& new_thermostat_data);

private:
    ThermostatData thermostat_data;///< An object keeping all necessary thermostat data.
};