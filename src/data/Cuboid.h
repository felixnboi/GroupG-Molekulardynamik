#pragma once

#include <array>

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
     * @brief Constructs a Cuboid with the given parameters.
     * 
     * @param position The coordinate of the lower left front-side corner in 3D space.
     * @param velocity Initial velocity of our cuboid's particles in 3D space.
     * @param dimensions Number of particles along each dimension in 3D space.
     * @param distance The distance between the particles.
     * @param mass The mass of each particle.
     */
    Cuboid(const std::array<double, 3>& position, const std::array<double, 3>& velocity, const std::array<unsigned, 3>& dimensions, double distance, double mass);

    /**
     * @brief Getter for the position of the Cuboid.
     * 
     * @return The position of the Cuboid.
     */
    std::array<double, 3> getPosition() const;

    /**
     * @brief Getter for the velocity of the Cuboid.
     * 
     * @return The velocity of the Cuboid.
     */
    std::array<double, 3> getVelocity() const;

    /**
     * @brief Getter for the dimensions of the Cuboid.
     * 
     * @return The dimensions of the Cuboid.
     */
    std::array<unsigned, 3> getDimensions() const;

    /**
     * @brief Getter for the distance between particles in the Cuboid.
     * 
     * @return The distance between particles in the Cuboid.
     */
    double getDistance() const;

    /**
     * @brief Getter for the mass of each particle in the Cuboid.
     * 
     * @return The mass of each particle in the Cuboid.
     */
    double getMass() const;

    /**
     * @brief Setter for the position of the Cuboid.
     * 
     * @param new_position The new position to set for the Cuboid.
     */
    void setPosition(const std::array<double, 3>& new_position);

    /**
     * @brief Setter for the velocity of the Cuboid.
     * 
     * @param new_velocity The new velocity to set for the Cuboid.
     */
    void setVelocity(const std::array<double, 3>& new_velocity);

    /**
     * @brief Setter for the dimensions of the Cuboid.
     * 
     * @param new_dimensions The new dimensions to set for the Cuboid.
     */
    void setDimensions(const std::array<unsigned, 3>& new_dimensions);

    /**
     * @brief Setter for the distance between particles in the Cuboid.
     * 
     * @param new_distance The new distance between particles to set for the Cuboid.
     */
    void setDistance(double new_distance);

    /**
     * @brief Setter for the mass of each particle in the Cuboid.
     * 
     * @param new_mass The new mass to set for each particle in the Cuboid.
     */
    void setMass(double new_mass);

    bool operator==(const Cuboid& other) const;

private:
    std::array<double, 3> position; ///< The position of the Cuboid.
    std::array<double, 3> velocity; ///< The velocity of the Cuboid.
    std::array<unsigned, 3> dimensions; ///< The dimensions of the Cuboid.
    double distance; ///< The distance between particles in the Cuboid.
    double mass; ///< The mass of each particle in the Cuboid.
};
