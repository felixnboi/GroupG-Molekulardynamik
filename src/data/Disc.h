#pragma once

#include <array>
#include <cstddef>

/**
 * @brief Represents a disc of Particles.
 */
class Disc {
public:

    /**
     * @brief Default constructor.
     */
    Disc();

    /**
     * @brief Constructor with parameters.
     * 
     * @param position The center of the disc.
     * @param velocity The initial velocity of the particles in the disc.
     * @param radius The radius of the disc  in terms of the number of molecules along the radius.
     * @param distance The distance between the partcles.
     * @param mass The mass of the particles.
     * @param epsilon The epsilon parameter for the particles of the disc.
     * @param sigma The sigma paramter for the particles of the disc.
     * @param brownian_motion_dimension The brownian motion average velocity dimension.
     * @param type The type of the particles in the disc.
     */
    Disc(const std::array<double, 3>& position, const std::array<double, 3>& velocity, int radius, double distance, double mass, 
    double epsilon, double sigma, size_t brownian_motion_dimension, int type);

    /**
     * @brief Getter for the position of the disc.
     * 
     * @return The position of the disc.
     */
    const std::array<double, 3>& getPosition() const;

    /**
     * @brief Getter for the velocity of each particle in the disc.
     * 
     * @return The velocity of each particle in the disc.
     */
    const std::array<double, 3>& getVelocity() const;

    /**
     * @brief Getter for the radius of the disc.
     * 
     * @return The radius of the disc.
     */
    int getRadius() const;

    /**
     * @brief Getter for the distance between particles in the disc.
     * 
     * @return The distance between particles in the disc.
     */
    double getDistance() const;

    /**
     * @brief Getter for the mass of each particle in the disc.
     * 
     * @return The mass of each particle in the disc.
     */
    double getMass() const;

    /**
     * @brief Getter for the value of epsilon of each particle in the disc.
     * 
     * @return The value of epsilon of each particle in the disc.
     */
    double getEpsilon() const;

    /**
     * @brief Getter for the value of sigma of each particle in the disc.
     * 
     * @return The value of sigma of each particle in the disc.
     */
    double getSigma() const;

    /**
     * @brief Getter for the dimension of average brownian motion velocity.
     * 
     * @return The dimension of average brownian motion velocity of the disc.
     */
    size_t getBrownianMotionDimension() const;

    /**
     * @brief Getter for the type of the particles in the disc.
     * 
     * @return The type of the particles in the disc.
     */
    int getType() const;

    /**
     * @brief Setter for the position of the disc.
     * 
     * @param new_position The new position of the disc.
     */
    void setPosition(const std::array<double, 3>& new_position);

    /**
     * @brief Setter for the velocity of each particle in the disc.
     * 
     * @param new_velocity The new velocity of each particle in the disc.
     */
    void setVelocity(const std::array<double, 3>& new_velocity);

    /**
     * @brief Setter for the radius of the disc.
     * 
     * @param new_radius The new radius of the disc.
     */
    void setRadius(int new_radius);

    /**
     * @brief Setter for the distance between particles in the disc.
     * 
     * @param new_distance The new distance of the disc.
     */
    void setDistance(double new_distance);

    /**
     * @brief Setter for the mass of each particle in the disc.
     * 
     * @param new_mass The new mass of each particle in the disc.
     */
    void setMass(double new_mass);

    /**
     * @brief Setter for epsilon of each particle in the disc.
     * 
     * @param new_epsilon The new epsilon of each particle in the disc.
     */
    void setEpsilon(double new_epsilon);

    /**
     * @brief Setter for sigma of each particle in the disc.
     * 
     * @param new_sigma The new sigma of each particle in the disc.
     */
    void setSigma(double new_sigma);

    /**
     * @brief Setter for the dimension of average brownian motion velocity.
     * 
     * @param new_brownian_motion_dimension The new dimension of average brownian motion velocity.
     */
    void setBrownianMotionDimension(size_t new_brownian_motion_dimension);

    /**
     * @brief Setter for the type of the particles in the disc.
     * 
     * @param new_type The new type of the particles in the disc.
     */
    void setType(int new_type);

    /**
     * @brief Equality operator for disc.
     * 
     * @param other The other disc to compare with.
     * 
     * @return True if the two discs are equal, false otherwise.
     */
    bool operator==(const Disc& other) const;

private:
    std::array<double, 3> position; ///< The position of the disc.
    std::array<double, 3> velocity; ///< The velocity of the disc.
    int radius; ///< The radius of the disc.
    double distance; ///< The distance of the disc.
    double mass; ///< The mass of the disc.
    double epsilon; ///< The parameter epsilon of the disc.
    double sigma; ///< The parameter sigma of the disc.
    size_t brownian_motion_dimension; ///< Parameter specifying the dimension of average brownian motion velocity of the disc.
    int type; ///< The type of the particles in the disc.
};
