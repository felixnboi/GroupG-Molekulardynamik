#pragma once

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

private:
    bool membrane_flag; ///< The flag indicating if the membrane simulation is being parsed.
    double r0; ///< The average bond length of a molecule pair.
    double k; ///< The stiffness constant.
    double f_z_up; ///< Force upwards along the z-axis.
};
