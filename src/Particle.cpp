/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <iostream>
#include "utils/ArrayUtils.h"
#include "spdlog/spdlog.h"


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
  spdlog::info("Particle generated with type {}", type_arg);
  f = {0., 0., 0.}; 
  old_f = {0., 0., 0.}; 
}

Particle::Particle(const Particle &other){
  x = other.x;
  v = other.v;
  f = other.f;
  old_f = other.old_f;
  m = other.m;
  type = other.type;
  spdlog::info("Particle generated by copy!");
}

// Todo: maybe use initializater list instead of copy?
Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, int type_arg){
  x = x_arg;
  v = v_arg;
  m = m_arg;
  type = type_arg;
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
  spdlog::info("Particle generated with position ({}, {}, {}) and velocity ({}, {}, {})",
               x_arg[0], x_arg[1], x_arg[2], v_arg[0], v_arg[1], v_arg[2]);
  spdlog::trace("type: {}, mass: {}", type_arg, m_arg);
}


Particle::~Particle() { 
  spdlog::info("Particle destructed!");
}

const std::array<double, 3> &Particle::getX() const { return x; }

const std::array<double, 3> &Particle::getV() const { return v; }

const std::array<double, 3> &Particle::getF() const { return f; }

const std::array<double, 3> &Particle::getOldF() const { return old_f; }

void Particle::setX(const std::array<double, 3>& newX) {
  x = newX;
}

void Particle::setV(const std::array<double, 3>& newV) {
  v = newV;
}

void Particle::setF(const std::array<double, 3>& newF) {
  f = newF;
}

void Particle::setOldF(const std::array<double, 3>& newOldF) {
  old_f = newOldF;
}

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

std::string Particle::toString() const {
  std::stringstream stream;
  stream << "Particle: X:" << x << " v: " << v << " f: " << f
         << " old_f: " << old_f << " type: " << type;
  return stream.str();
}


bool Particle::operator==(const Particle &other) const {
  return (x == other.x) and (v == other.v) and (f == other.f) and
         (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream & operator<<(std::ostream &stream, const Particle &p) {
  stream << p.toString();
  return stream;
}