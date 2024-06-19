#include "Cuboid.h"

Cuboid::Cuboid(){}

Cuboid::Cuboid(const std::array<double, 3>& position, const std::array<double, 3>& velocity, const std::array<unsigned, 3>& dimensions, 
double distance, double mass, double brownian_motion, double epsilon, double sigma, size_t brownian_motion_dimension, size_t type)
    : position(position),
      velocity(velocity),
      dimensions(dimensions),
      distance(distance), 
      mass(mass), 
      brownian_motion(brownian_motion), 
      epsilon(epsilon), 
      sigma(sigma), 
      brownian_motion_dimension(brownian_motion_dimension), 
      type(type) {}

std::array<double, 3> Cuboid::getPosition() const { return position; }
std::array<double, 3> Cuboid::getVelocity() const { return velocity; }
std::array<unsigned, 3> Cuboid::getDimensions() const { return dimensions; }
double Cuboid::getDistance() const { return distance; }
double Cuboid::getMass() const { return mass; }
double Cuboid::getBrownianMotion() const { return brownian_motion; }
double Cuboid::getEpsilon() const{ return epsilon; }
double Cuboid::getSigma() const{ return sigma; }
size_t Cuboid::getBrownianMotionDimension() const{ return brownian_motion_dimension; }
size_t Cuboid::getType() const{ return type; }

void Cuboid::setPosition(const std::array<double, 3>& new_position) { position = new_position; }
void Cuboid::setVelocity(const std::array<double, 3>& new_velocity) { velocity = new_velocity; }
void Cuboid::setDimensions(const std::array<unsigned, 3>& new_dimensions) { dimensions = new_dimensions; }
void Cuboid::setDistance(double new_distance) { distance = new_distance; }
void Cuboid::setMass(double new_mass) { mass = new_mass; }
void Cuboid::setBrownianMotion(double new_brownian_motion) { brownian_motion = new_brownian_motion; }
void Cuboid::setEpsilon(double new_epsilon) { epsilon = new_epsilon; }
void Cuboid::setSigma(double new_sigma) { sigma = new_sigma; }
void Cuboid::setBrownianMotionDimension(size_t new_brownian_motion_dimension) { brownian_motion_dimension = new_brownian_motion_dimension; }
void Cuboid::setType(size_t new_type) { type = new_type; }

bool Cuboid::operator==(const Cuboid& other) const{
    return position == other.position && velocity == other.velocity && dimensions == other.dimensions 
        && distance == other.distance && mass == other.mass && brownian_motion == other.brownian_motion 
        && epsilon == other.epsilon && sigma == other.sigma && brownian_motion_dimension == other.brownian_motion_dimension 
        && type == other.type;
}
