#pragma once

#include <array>

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
     * @param position The center of the disc.
     * @param velocity The initial velocity of the particles in the disc.
     * @param radius The radius of the disc  in terms of the number of molecules along the radius.
     * @param distance The distance between the partcles.
     * @param mass The mass of the particles.
     */
    Disc(const std::array<double, 3>& position, const std::array<double, 3>& velocity, int radius, double distance, double mass);

    /**
     * @brief Getter for the position of the disc.
     * @return The position of the disc.
     */
    const std::array<double, 3>& getPosition() const;

    /**
     * @brief Getter for the velocity of the disc.
     * @return The velocity of the disc.
     */
    const std::array<double, 3>& getVelocity() const;

    /**
     * @brief Getter for the radius of the disc.
     * @return The radius of the disc.
     */
    int getRadius() const;
    

    /**
     * @brief Getter for the distance of the disc.
     * @return The distance of the disc.
     */
    double getDistance() const;

    /**
     * @brief Getter for the mass of the disc.
     * @return The mass of the disc.
     */
    double getMass() const;

    /**
     * @brief Setter for the position of the disc.
     * @param position The new position of the disc.
     */
    void setPosition(const std::array<double, 3>& position);

    /**
     * @brief Setter for the velocity of the disc.
     * @param velocity The new velocity of the disc.
     */
    void setVelocity(const std::array<double, 3>& velocity);

    /**
     * @brief Setter for the distance of the disc.
     * @param distance The new distance of the disc.
     */
    void setDistance(double distance);

    /**
     * @brief Setter for the mass of the disc.
     * @param mass The new mass of the disc.
     */
    void setMass(double mass);

    /**
     * @brief Setter for the radius of the disc.
     * @param radius The new radius of the disc.
     */
    void setRadius(int radius);

    bool operator==(const Disc& other) const;

private:
    std::array<double, 3> position; //<<< The position of the disc.
    std::array<double, 3> velocity; //<<< The velocity of the disc.
    int radius; //<<< The radius of the disc.
    double distance; //<<< The distance of the disc.
    double mass; //<<< The mass of the disc.
};
