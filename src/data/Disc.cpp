#include "Disc.h"

Disc::Disc()
    : position({0.0, 0.0, 0.0}), velocity({0.0, 0.0, 0.0}), radius(0), distance(0.0), mass(0.0) {}

Disc::Disc(const std::array<double, 3>& position, const std::array<double, 3>& velocity, int radius, double distance, double mass, 
double epsilon, double sigma, size_t brownian_motion_dimension, int type)
    : position(position), 
      velocity(velocity), 
      radius(radius), 
      distance(distance), 
      mass(mass), 
      epsilon(epsilon), 
      sigma(sigma), 
      brownian_motion_dimension(brownian_motion_dimension), 
      type(type) {}

const std::array<double, 3>& Disc::getPosition() const { return position; }
const std::array<double, 3>& Disc::getVelocity() const { return velocity; }
int Disc::getRadius() const { return radius; }
double Disc::getDistance() const { return distance; }
double Disc::getMass() const { return mass; }
double Disc::getEpsilon() const{ return epsilon; }
double Disc::getSigma() const{ return sigma; }
size_t Disc::getBrownianMotionDimension() const{ return brownian_motion_dimension; }
int Disc::getType() const{ return type; }

void Disc::setPosition(const std::array<double, 3>& new_position) { position = new_position; }
void Disc::setVelocity(const std::array<double, 3>& new_velocity) { velocity = new_velocity; }
void Disc::setDistance(double new_distance) { distance = new_distance; }
void Disc::setMass(double new_mass) { mass = new_mass; }
void Disc::setRadius(int new_radius) { radius = new_radius; }
void Disc::setEpsilon(double new_epsilon) { epsilon = new_epsilon; }
void Disc::setSigma(double new_sigma) { sigma = new_sigma; }
void Disc::setBrownianMotionDimension(size_t new_brownian_motion_dimension) { brownian_motion_dimension = new_brownian_motion_dimension; }
void Disc::setType(int new_type) { type = new_type; }

bool Disc::operator==(const Disc& other) const{
    return position == other.position && velocity == other.velocity && radius == other.radius && distance == other.distance 
    && mass == other.mass && epsilon == other.epsilon && sigma == other.sigma && brownian_motion_dimension == other.brownian_motion_dimension 
    && type == other.type;
}
