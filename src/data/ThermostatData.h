#pragma once

#include <cmath>

/**
 * @brief Class representing the data used by thermostat.
 */
class ThermostatData {
public:
    /**
     * @brief Default constructor.
     */
    ThermostatData();

    /**
    * @brief Parameterized constructor for the ThermostatData class.
    * 
    * @param thermostat_flag Flag indicating if the thermostat is active.
    * @param n_thermostat Number of thermostat iterations.
    * @param brownian_motion_dimension Dimension of Brownian motion.
    * @param target_temp_flag Flag indicating if target temperature is set.
    * @param target_temp Target temperature.
    * @param max_delta_temp Maximum change in temperature.
    * @param init_temp_flag Flag indicating if initial temperature is set.
    * @param init_temp Initial temperature.
    */
    ThermostatData(bool thermostat_flag, size_t n_thermostat, size_t brownian_motion_dimension, bool target_temp_flag, 
    double target_temp, double max_delta_temp, bool init_temp_flag, double init_temp);

    /**
     * @brief Getter for the thermostat flag.
     *
     * @return The thermostat flag.
     */
    bool getThermostatFlag() const;

    /**
     * @brief Getter for the number of iterations between thermostat applications.
     *
     * @return The number of iterations between thermostat applications.
     */
    size_t getNThermostat() const;

    /**
     * @brief Getter for the average brownian motion velocity dimension.
     *
     * @return The average brownian motion velocity dimension.
     */
    size_t getBrownianMotionDimension() const;

    /**
     * @brief Getter for the target temperature flag.
     *
     * @return The target temperature flag.
     */
    bool getTargetTempFlag() const;

    /**
     * @brief Getter for the target temperature of the thermostat.
     *
     * @return The target temperature of the thermostat.
     */
    double getTargetTemp() const;

    /**
     * @brief Getter for the maximum temperature change of the thermostat.
     *
     * @return The maximum temperature change of the thermostat.
     */
    double getMaxDeltaTemp() const;

    /**
     * @brief Getter for the initial temperature flag.
     *
     * @return The initial temperature flag.
     */
    bool getInitTempFlag() const;

    /**
     * @brief Getter for the initial temperature.
     *
     * @return The initial temperature.
     */
    double getInitTemp() const;

    /**
     * @brief Setter for the thermostat flag.
     *
     * @param new_thermostat_flag The new thermostatflag.
     */
    void setThermostatFlag(bool new_thermostat_flag);

    /**
     * @brief Setter for n_thermostat.
     *
     * @param new_n_thermostat The new n_thermostat.
     */
    void setNThermostat(size_t new_n_thermostat);

    /**
     * @brief Setter for the brownian motion average velocity dimension.
     *
     * @param new_brownian_motion_dimension The new brownian motion average velocity dimension.
     */
    void setBrownianMotionDimension(size_t new_brownian_motion_dimension);

    /**
     * @brief Setter for the target temperature flag.
     *
     * @param new_target_temp_flag The new target temperature flag.
     */
    void setTargertTempFlag(bool new_target_temp_flag);

    /**
     * @brief Setter for the target temperature.
     *
     * @param new_target_temp The new traget temperature.
     */
    void setTargetTemp(double new_target_temp);

    /**
     * @brief Setter for the maximum delta temperature.
     *
     * @param new_max_delta_temp The new maximum delta temperature.
     */
    void setMaxDeltaTemp(double new_max_delta_temp);

    /**
     * @brief Setter for the initial temperature flag.
     *
     * @param new_init_temp_flag The new initial temperature flag.
     */
    void setInitTempFlag(bool new_init_temp_flag);

    /**
     * @brief Setter for the initial temperature.
     *
     * @param new_init_temp The new initial temperature.
     */
    void setInitTemp(double new_init_temp);

private:
    bool thermostat_flag; ///<The flag specifying if the thermostat is used.
    size_t n_thermostat; ///<The number of iterations between thermostat applications.
    size_t brownian_motion_dimension; ///<Thermostat needs to know the average brownian motion velocity dimension for the temperature-computation
    bool target_temp_flag; ///<The flag specifying if a target temperature was set.
    double target_temp; ///<Target temperature that the Thermostat tries to reach.
    double max_delta_temp; ///<Determines the maximum temperature manipulation in one iteration step.
    bool init_temp_flag; ///<The flag specifying if a initial temperature was set.
    double init_temp; ///<Initial temperature.
};