#include "Cuboid.h"

Cuboid::Cuboid(){}

Cuboid::Cuboid(const std::array<double, 3>& position, const std::array<double, 3>& velocity, const std::array<unsigned, 3>& dimensions, double distance, double mass)
    : position(position), velocity(velocity), dimensions(dimensions), distance(distance), mass(mass) {}

std::array<double, 3> Cuboid::getPosition() const {
    return position;
}

std::array<double, 3> Cuboid::getVelocity() const {
    return velocity;
}

std::array<unsigned, 3> Cuboid::getDimensions() const {
    return dimensions;
}

double Cuboid::getDistance() const {
    return distance;
}

double Cuboid::getMass() const {
    return mass;
}

void Cuboid::setPosition(const std::array<double, 3>& new_position) {
    position = new_position;
}

void Cuboid::setVelocity(const std::array<double, 3>& new_velocity) {
    velocity = new_velocity;
}

void Cuboid::setDimensions(const std::array<unsigned, 3>& new_dimensions) {
    dimensions = new_dimensions;
}

void Cuboid::setDistance(double new_distance) {
    distance = new_distance;
}

void Cuboid::setMass(double new_mass) {
    mass = new_mass;
}
