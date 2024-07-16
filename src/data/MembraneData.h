#pragma once

#include <utility>
#include <vector>
#include <stddef.h>

/**
 * @class MembraneData
 * @brief Class to handle the membrane simulation.
 */
class MembraneData {
public:
    /**
     * @brief Default constructor
     */
    MembraneData();

    /**
     * @brief Constructor with full parameters
     * 
     * @param membrane_flag The flag indicating if a membrane is being parsed
     * @param r0 The initial radius
     * @param k The stiffness constant
     * @param f_z_up The upward force
     */
    MembraneData(bool membrane_flag, double r0, double k, double f_z_up);

    /**
     * @brief Getter for the membrane flag
     * 
     * @return The membrane flag
     */
    bool getMembraneFlag() const;

    /**
     * @brief Getter for the average bond length of a molecule pair
     * 
     * @return The average bond length of a molecule pair
     */
    double getR0() const;

    /**
     * @brief Getter for the stiffness constant
     * 
     * @return The stiffness constant
     */
    double getK() const;

    /**
     * @brief Getter for the upward force
     * 
     * @return The upward force
     */
    double getF_z_up() const;

    /**
     * @brief Getter for the vector of pairs
     * 
     * @return A constant reference to the vector of pairs
     */
    const std::vector<std::pair<size_t, size_t>>& getParticleUp() const;

    /**
     * @brief Setter for the membrane flag
     * 
     * @param new_membrane_flag The new membrane flag
     */
    void setMembraneFlag(bool new_membrane_flag);

    /**
     * @brief Setter for the average bond length of a molecule pair
     * 
     * @param new_r0 The new average bond length of a molecule pair
     */
    void setR0(double new_r0);

    /**
     * @brief Setter for the stiffness constant
     * 
     * @param new_k The new stiffness constant
     */
    void setK(double new_k);

    /**
     * @brief Setter for the upward force
     * 
     * @param new_f_z_up The new upward force
     */
    void setF_z_up(double new_f_z_up);

    /**
     * @brief Setter for the vector of pairs
     * 
     * @param pairs A constant reference to a vector of pairs to set
     */
    void setParticleUp(const std::vector<std::pair<size_t, size_t>>& new_particle_up);

    /**
     * @brief Adds a particle to the particle_up vector.
     * 
     * @param x_index The x_index of the particle
     * @param y_index The y_index of the particle
     */
    void addParticleUp(size_t x_index, size_t y_index);

private:
    bool membrane_flag; ///< The flag indicating if the membrane simulation is being parsed.
    double r0; ///< The average bond length of a molecule pair.
    double k; ///< The stiffness constant.
    double f_z_up; ///< Force upwards along the z-axis.
    std::vector<std::pair<size_t, size_t>> particle_up; ///< The vector storing the indices of the particles that are going to be influenced by the upward force.
};
