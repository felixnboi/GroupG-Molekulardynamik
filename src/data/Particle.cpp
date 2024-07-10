/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

/**
 * @brief Constructs a particle with a specified type.
 * 
 * This constructor initializes a particle object with the provided type. 
 * It sets the type member variable to the value of type_arg.
 * Additionally, it initializes the force (f) and old force (old_f) arrays to {0., 0., 0.}.
 * Finally, it outputs a message indicating that the particle has been generated.
 * 
 * @param type_arg The type of the particle.
 */
Particle::Particle(int type_arg) {
  type = type_arg; 
  f = {0., 0., 0.}; 
  old_f = {0., 0., 0.}; 
  domainStart = {0., 0., 0.}; 
  is_outer = false;
  epsilon = 5;
  sigma = 1;
  spdlog::info("Particle generated with type {}", type_arg);
}

Particle::Particle(const Particle &other){
  x = other.x;
  v = other.v;
  f = other.f;
  domainStart = other.domainStart;
  epsilon = other.epsilon;
  sigma = other.sigma;
  old_f = other.old_f;
  m = other.m;
  is_outer = other.is_outer;
  type = other.type;
  spdlog::info("Particle generated by copy!");
}

// Todo: maybe use initializater list instead of copy?
Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, bool is_outer_arg, int type_arg, double epsilon, double sigma, 
std::array<double,3> domainStart){
  this->sigma = sigma;
  this->epsilon = epsilon;
  this->domainStart = domainStart;
  x = x_arg-domainStart;
  v = v_arg;
  m = m_arg;
  is_outer = is_outer_arg;
  type = type_arg;
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
  spdlog::info("Particle generated with position ({}, {}, {}) and velocity ({}, {}, {})",
               x_arg[0], x_arg[1], x_arg[2], v_arg[0], v_arg[1], v_arg[2]);
  spdlog::trace("type: {}, mass: {}", type_arg, m_arg);
}


Particle::~Particle() {
  // Cannot use logging because at end of main logger is destructed before particles -> Would lead to use-after-free;
}

const std::array<double, 3> &Particle::getX() const { return x; }

const std::array<double, 3> &Particle::getV() const { return v; }

const std::array<double, 3> &Particle::getF() const { return f; }

const std::array<double, 3> &Particle::getDomainStart() const { return domainStart; }

const std::array<double, 3> &Particle::getOldF() const { return old_f; }

const bool Particle::getIsOuter() const { return is_outer; }

void Particle::setX(const std::array<double, 3>& newX) {
  x = newX;
}

void Particle::setV(const std::array<double, 3>& newV) {
  v = newV;
}

void Particle::setF(const std::array<double, 3>& newF) {
  if(!is_outer){
    f = newF;
  }
}

void Particle::setIsOuter(bool newIsOuter) {
  is_outer = newIsOuter;
}

void Particle::applyF(const std::array<double, 3>& force){
  if(!is_outer){
    f = f+force;
  }
}

void Particle::setOldF(const std::array<double, 3>& newOldF) {
  if(!is_outer){
    old_f = newOldF;
  }
}

double Particle::getM() const { return m; }

const std::array<std::shared_ptr<Particle>,4> &Particle::getNeighbours() const {return neighbours;}

const std::array<bool,4> &Particle::getHasNeighbour() const {return hasNeighbour;}

int Particle::getType() const { return type; }

std::string Particle::toString() const {
  std::stringstream stream;
  stream << "Particle: X:" << x << " v: " << v << " f: " << f
         << " old_f: " << old_f << " type: " << type;
  return stream.str();
}

const double Particle::getEpsilon() const{
  return epsilon;
}

const double Particle::getSigma() const{
  return sigma;
}

bool Particle::operator==(const Particle &other) const {
  return (x == other.x) and (v == other.v) and (f == other.f) and
         (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream & operator<<(std::ostream &stream, const Particle &p) {
  stream << p.toString();
  return stream;
}

void Particle::addNeighbour(std::shared_ptr<Particle> neighbour, int position){
  neighbours[position] = neighbour;
  hasNeighbour[position] = true;
}