#pragma once

#include <string>
#include <array>

/**
 * @brief Class representing simulation data parameters.
 */
class SimData {
public:
    /**
     * @brief Default constructor.
     */
    SimData();

    /**
     * @brief Constructor with full parameter list.
     * 
     * @param input_file The input file name.
     * @param baseName The base name for simulation.
     * @param write_frequency Frequency of writing simulation data.
     * @param start_time Start time of the simulation.
     * @param end_time End time of the simulation.
     * @param delta_t Time step size of the simulation.
     * @param force_str String representation of the force used in simulation.
     * @param algorithm The algorithm used for simulation.
     * @param loglevel The level of logging for simulation.
     * @param boundary Array containing boundary information.
     * @param cutoff_radius The cutoff radius used in simulation.
     * @param dimensions The number of dimensions of simulation (could be 2 or 3).
     * @param domain Array containing domain information.
     * @param domain_start Array containing the starting position of the domain.
     * @param gravConstant The gravitational constant.
     * @param walls_flag The walls flag.
     */
    SimData(std::string input_file, std::string baseName, unsigned write_frequency, double start_time, double end_time, double delta_t, 
    std::string force_str, std::string algorithm, std::string loglevel, std::array<std::string, 6> boundary, double cutoff_radius, 
    size_t dimensions, std::array<double, 3> domain, std::array<double, 3> domain_start, std::array<double, 3> gravConstant, 
    bool walls_flag);
            
    /**
     * @brief Destructor.
     */
    ~SimData();
    
    /**
     * @brief Getter for the input file name.
     * 
     * @return The input file name.
     */
    std::string getInputFile() const;

    /**
     * @brief Getter for the base name for simulation.
     * 
     * @return The base name for simulation.
     */
    std::string getBaseName() const;

    /**
     * @brief Getter for the frequency of writing simulation data.
     * 
     * @return The frequency of writing simulation data.
     */
    unsigned getWriteFrequency() const;

    /**
     * @brief Getter for the start time of the simulation.
     * 
     * @return The start time of the simulation.
     */
    double getStartTime() const;

    /**
     * @brief Getter for the end time of the simulation.
     * 
     * @return The end time of the simulation.
     */
    double getEndTime() const;

    /**
     * @brief Getter for the time step size of the simulation.
     * 
     * @return The time step size of the simulation.
     */
    double getDeltaT() const;

    /**
     * @brief Getter for the string representation of the force used in simulation.
     * 
     * @return The string representation of the force used in simulation.
     */
    std::string getForceStr() const;

    /**
     * @brief Getter for the algorithm used for simulation.
     * 
     * @return The algorithm used for simulation.
     */
    std::string getAlgorithm() const;

    /**
     * @brief Getter for the level of logging for simulation.
     * 
     * @return The level of logging for simulation.
     */
    std::string getLoglevel() const;

    /**
     * @brief Getter for the array containing boundary information.
     * 
     * @return The array containing boundary information.
     */
    std::array<std::string, 6> getBoundary() const;

    /**
     * @brief Getter for the cutoff radius used in simulation.
     * 
     * @return The cutoff radius used in simulation.
     */
    double getCutoffRadius() const;

    /**
     * @brief Getter for the array containing number of dimensions.
     * 
     * @return The array containing dimensions information.
     */
    double getDimensions() const;

    /**
     * @brief Getter for the array containing domain information.
     * 
     * @return The array containing domain information.
     */
    std::array<double, 3> getDomain() const;

    /**
     * @brief Getter for the array containing domain start.
     * 
     * @return The array containing domain start.
     */
    std::array<double, 3> getDomainStart() const;

    /**
     * @brief Getter for the gravitational constant.
     * 
     * @return The gravitational constant used in the simulation.
     */
    std::array<double, 3> getGravConstant() const;

    /**
     * @brief Getter for the walls flag.
     * 
     * @return The walls flag of the simulation.
     */
    bool getWallsFlag() const;

    /**
     * @brief Setter for the input file name.
     * 
     * @param new_input_file The new input file name.
     */
    void setInputFile(const std::string& new_input_file);

    /**
     * @brief Setter for the base name for simulation.
     * 
     * @param new_base_name The new base name for simulation.
     */
    void setBaseName(const std::string& new_base_name);

    /**
     * @brief Setter for the frequency of writing simulation data.
     * 
     * @param new_write_frequency The new frequency of writing simulation data.
     */
    void setWriteFrequency(unsigned new_write_frequency);

    /**
     * @brief Setter for the start time of the simulation.
     * 
     * @param new_start_time The new start time of the simulation.
     */
    void setStartTime(double new_start_time);

    /**
     * @brief Setter for the end time of the simulation.
     * 
     * @param new_end_time The new end time of the simulation.
     */
    void setEndTime(double new_end_time);

    /**
     * @brief Setter for the time step size of the simulation.
     * 
     * @param new_delta_t The new time step size of the simulation.
     */
    void setDeltaT(double new_delta_t);

    /**
     * @brief Setter for the string representation of the force used in simulation.
     * 
     * @param new_force_str The new string representation of the force used in simulation.
     */
    void setForceStr(const std::string& new_force_str);

    /**
     * @brief Setter for the algorithm used for simulation.
     * 
     * @param new_algorithm The new algorithm used for simulation.
     */
    void setAlgorithm(const std::string& new_algorithm);

    /**
     * @brief Setter for the level of logging for simulation.
     * 
     * @param new_loglevel The new level of logging for simulation.
     */
    void setLoglevel(const std::string& new_loglevel);

    /**
     * @brief Setter for the array containing boundary information.
     * 
     * @param new_boundary The new array containing boundary information.
     */
    void setBoundary(const std::array<std::string, 6>& new_boundary);

    /**
     * @brief Setter for the cutoff radius used in simulation.
     * 
     * @param new_cutoff_radius The new cutoff radius used in simulation.
     */
    void setCutoffRadius(double new_cutoff_radius);

    /**
     * @brief Setter for the number of dimensions used in simulation.
     * 
     * @param new_dimensions The new number of dimensions used in simulation.
     */
    void setDimensions(double new_dimensions);

    /**
     * @brief Setter for the array containing domain information.
     * 
     * @param new_domain The new array containing domain information.
     */
    void setDomain(const std::array<double, 3>& new_domain);

    /**
     * @brief Setter for the array containing domain start.
     * 
     * @param new_domain_start The new array containing domain start.
     */
    void setDomainStart(const std::array<double, 3>& new_domain_start);

    /**
     * @brief Setter for the gravitational constant.
     * 
     * @param new_gravconstant The new gravitational constant used in simulation.
     */
    void setGravConstant(const std::array<double, 3> new_grav_constant);

    /**
     * @brief Setter for the walls_flag parameter.
     * 
     * @param new_walls_flag The new walls_flag value used in simulation.
     */
    void setWallsFlag(bool new_walls_flag);


private:
    std::string input_file; ///< The input file name.
    std::string baseName; ///< The base name for simulation.
    unsigned write_frequency; ///< Frequency of writing simulation data.
    double start_time; ///< Start time of the simulation.
    double end_time; ///< End time of the simulation.
    double delta_t; ///< Time step size of the simulation.
    std::string force_str; ///< String representation of the force used in simulation.
    std::string algorithm; ///< The algorithm used for simulation.
    std::string loglevel; ///< The level of logging for simulation.
    std::array<std::string, 6> boundary; ///< Array containing boundary information.
    double cutoff_radius; ///< The cutoff radius used in simulation.
    size_t dimensions; ///<  Number of dimensions used in simulation.
    std::array<double, 3> domain; ///< Array containing domain information.
    std::array<double, 3> domain_start; ///< Array containing the starting position of the domain.
    std::array<double, 3> gravConstant; ///< Gravitational constant for the Simulation.
    bool walls_flag; ///< Flag indicating whether the walls in simulation are present.
};

