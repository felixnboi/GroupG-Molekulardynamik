#pragma once

#include <array>
#include <cstddef>

/**
 * @brief This class describes the needed parameters of a cuboid of particles.
 */
class Cuboid {
public:
    
    /**
     * @brief Default constructor.
     */
    Cuboid();

    /**
     * @brief Constructs a cuboid with the given parameters.
     * 
     * @param position The coordinate of the lower left front-side corner in 3D space.
     * @param velocity Initial velocity of our cuboid's particles in 3D space.
     * @param dimensions Number of particles along each dimension in 3D space.
     * @param distance The distance between the particles.
     * @param mass The mass of each particle.
     * @param brownian_motion The brownian motion average velocity.
     * @param epsilon The epsilon parameter for the particles of the cuboid.
     * @param sigma The sigma paramter for the particles of the cuboid.
     * @param brownian_motion_dimension The brownian motion average velocity dimension.
     * @param type The type of the particles in the cuboid.
     * @param is_outer Whether the cuboid is outer cuboid(wall).
     */
    Cuboid(const std::array<double, 3>& position, const std::array<double, 3>& velocity, const std::array<unsigned, 3>& dimensions, 
    double distance, double mass, double brownian_motion, double epsilon, double sigma, size_t brownian_motion_dimension, int type, bool is_outer);

    /**
     * @brief Getter for the position of the cuboid.
     * 
     * @return The position of the cuboid.
     */
    std::array<double, 3> getPosition() const;

    /**
     * @brief Getter for the velocity of each particle in the cuboid.
     * 
     * @return The velocity of the cuboid.
     */
    std::array<double, 3> getVelocity() const;

    /**
     * @brief Getter for the dimensions of the cuboid.
     * 
     * @return The dimensions of the cuboid.
     */
    std::array<unsigned, 3> getDimensions() const;

    /**
     * @brief Getter for the distance between particles in the cuboid.
     * 
     * @return The distance between particles in the cuboid.
     */
    double getDistance() const;

    /**
     * @brief Getter for the mass of each particle in the cuboid.
     * 
     * @return The mass of each particle in the cuboid.
     */
    double getMass() const;

    /**
     * @brief Getter for the brownian motion of the cuboid.
     * 
     * @return The brownian motion average velocity of the cuboid.
     */
    double getBrownianMotion() const;

    /**
     * @brief Getter for the value of epsilon of each particle in the cuboid.
     * 
     * @return The value of epsilon of each particle in the cuboid.
     */
    double getEpsilon() const;

    /**
     * @brief Getter for the value of sigma of each particle in the cuboid.
     * 
     * @return The value of sigma of each particle in the cuboid.
     */
    double getSigma() const;

    /**
     * @brief Getter for the dimension of average brownian motion velocity.
     * 
     * @return The dimension of average brownian motion velocity of the cuboid.
     */
    size_t getBrownianMotionDimension() const;

    /**
     * @brief Getter for the type of the particles in the cuboid.
     * 
     * @return The type of the particles in the cuboid.
     */
    int getType() const;

    /**
     * @brief Getter for the is_outer parameter of the particles in the cuboid.
     * 
     * @return The is_outer of the particles in the cuboid.
     */
    bool getIsOuter() const;
    
    /**
     * @brief Setter for the position of the cuboid.
     * 
     * @param new_position The new position to set for the cuboid.
     */
    void setPosition(const std::array<double, 3>& new_position);

    /**
     * @brief Setter for the velocity of each particle in the cuboid.
     * 
     * @param new_velocity The new velocity to set for each particle in the cuboid.
     */
    void setVelocity(const std::array<double, 3>& new_velocity);

    /**
     * @brief Setter for the dimensions of the cuboid.
     * 
     * @param new_dimensions The new dimensions to set for the cuboid.
     */
    void setDimensions(const std::array<unsigned, 3>& new_dimensions);

    /**
     * @brief Setter for the distance between particles in the cuboid.
     * 
     * @param new_distance The new distance between particles to set for the cuboid.
     */
    void setDistance(double new_distance);

    /**
     * @brief Setter for the mass of each particle in the cuboid.
     * 
     * @param new_mass The new mass to set for each particle in the cuboid.
     */
    void setMass(double new_mass);

    /**
     * @brief Setter for the brownian motion of each particle in the cuboid.
     * 
     * @param new_brownian_motion The new brownian motion average velocity to set for the cuboid.
     */
    void setBrownianMotion(double new_brownian_motion);

    /**
     * @brief Setter for sigma of each particle in the cuboid.
     * 
     * @param new_epsilon The new sigma to set for each particle in the cuboid.
     */
    void setEpsilon(double new_epsilon);

    /**
     * @brief Setter for sigma of each particle in the cuboid.
     * 
     * @param new_sigma The new sigma to set for each particle in the cuboid.
     */
    void setSigma(double new_sigma);

    /**
     * @brief Setter for the dimension of average brownian motion velocity.
     * 
     * @param new_brownian_motion_dimension The new dimension of average brownian motion velocity.
     */
    void setBrownianMotionDimension(size_t new_brownian_motion_dimension);

    /**
     * @brief Setter for the type of the particles in the cuboid.
     * 
     * @param new_type The new type of the particles in the cuboid.
     */
    void setType(int new_type);

    /**
     * @brief Setter for the is_outer of the particles in the cuboid.
     * 
     * @param new_is_outer The new is_outer of the particles in the cuboid.
     */
    void setIsOuter(int new_is_outer);

    /**
     * @brief Equality operator for cuboid.
     * 
     * @param other The other cuboid to compare with.
     * @return True if the two cuboids are equal, false otherwise.
     */
    bool operator==(const Cuboid& other) const;

private:
    std::array<double, 3> position; ///< The position of the cuboid.
    std::array<double, 3> velocity; ///< The velocity of the cuboid.
    std::array<unsigned, 3> dimensions; ///< The dimensions of the cuboid.
    double distance; ///< The distance between particles in the cuboid.
    double mass; ///< The mass of each particle in the cuboid.
    double brownian_motion; ///< The brownian motion average velocity.
    double epsilon; ///< The paramter epsilon of the cuboid.
    double sigma; ///< The parameter sigma of the cuboid.
    size_t brownian_motion_dimension; ///< Parameter specifying the dimension of average brownian motion velocity of the cuboid.
    int type; ///<The type of the particles in the cuboid.
    bool is_outer; ///< Whether the cuboid is outer cuboid(wall).
};
