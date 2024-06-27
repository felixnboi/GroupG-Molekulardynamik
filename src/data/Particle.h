/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "../utils/ArrayUtils.h"
#include "spdlog/spdlog.h"

#include <array>
#include <string>
#include <iostream>


/**
 * @brief Represents a particle in a simulation.
 */
class Particle {

private:
  /**
   * Position of the particle
   */
  std::array<double, 3> x;

  /**
   * Velocity of the particle
   */
  std::array<double, 3> v;

  /**
   * Force effective on this particle
   */
  std::array<double, 3> f;

  /**
   * Force which was effective on this particle
   */
  std::array<double, 3> old_f;

  /**
   * Mass of this particle
   */
  double m;

  /**
   * Type of the particle. Use it for whatever you want (e.g. to separate
   * molecules belonging to different bodies, matters, and so on)
   */
  int type;

  bool is_outer;

  double sigma; ///< The sigma value for the Lennard-Jones potential.
  double epsilon; ///< The epsilon value for the Lennard-Jones potential.
  std::array<double, 3> domainStart; ///< The domain start position of the particle.


public:
  /**
   * @brief Constructs a particle with a given type.
   * 
   * @param type The type of the particle.
   */
  explicit Particle(int type = 0);

  /**
   * @brief Copy constructor.
   * 
   * @param other The particle to copy from.
   */
  Particle(const Particle &other);

  /**
   * @brief Constructs a particle with given parameters.
    * 
   * @param x_arg The initial position of the particle.
   * @param v_arg The initial velocity of the particle.
   * @param m_arg The mass of the particle.
   * @param is_outer_arg Whether the particle belongs to the outer cuboid (wall).
   * @param type_arg The type of the particle.
   * @param epsilon The epsilon value for the Lennard-Jones potential.
   * @param sigma The sigma value for the Lennard-Jones potential.
   * @param domainStart The domain start position of the particle.
   */
  Particle(
      // for visualization, we need always 3 coordinates
      // -> in case of 2d, we use only the first and the second
      std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, bool is_outer_arg,
      int type_arg, double epsilon, double sigma, std::array<double, 3> domainStart);

  /**
   * @brief Destructor.
   */
  virtual ~Particle();

  /**
   * @brief Gets the position of the particle.
   * 
   * @return Reference to the position array.
   */
  const std::array<double, 3> &getX() const;

  /**
   * @brief Gets the velocity of the particle.
   * 
   * @return Reference to the velocity array.
   */
  const std::array<double, 3> &getV() const;

  /**
   * @brief Gets the force acting on this particle.
   * 
   * @return Reference to the force array.
   */
  const std::array<double, 3> &getF() const;

  /**
   * @brief Gets the force which was acting on this particle in the previous time stamp.
   * 
   * @return Reference to the old force array.
   */
  const std::array<double, 3> &getOldF() const;

  /**
   * @brief Gets the is_outer parameter of the particle.
   * 
   * @return Boolean value of is_outer.
   */
  const bool getIsOuter() const;

  /**
   * @brief Gets the epsilon parameter of the particle.
   * 
   * @return Double value of epsilon.
   */
  const double getEpsilon() const;

  /**
   * @brief Gets the sigma parameter of the particle.
   * 
   * @return Double value of sigma.
   */
  const double getSigma() const;

  /**
   * @brief Gets the domain start coordinates.
   * 
   * @return Reference to the coordinates array.
   */
  const std::array<double, 3> &getDomainStart() const;

  /**
   * @brief Sets the position of the particle.
   * 
   * @param newX New position array.
   */
  void setX(const std::array<double, 3>& newX);

  /**
   * @brief Sets the is_outer parameter of the particle.
   * 
   * @param newIsOuter New is_outer value.
   */
  void setIsOuter(const bool newIsOuter);

  /**
   * @brief Sets the velocity of the particle.
   * 
   * @param newV New velocity array.
   */
  void setV(const std::array<double, 3>& newV);

  /**
   * @brief Sets the force that is acting on this particle.
   * 
   * @param newF New force array.
   */
  void setF(const std::array<double, 3>& newF);

  void applyF(const std::array<double, 3>& force);
  /**
   * @brief Sets the old force on this particle.
   * 
   * @param newOldF New old force array.
   */
  void setOldF(const std::array<double, 3>& newOldF);

  /**
   * @brief Gets the mass of the particle.
   * 
   * @return The mass of the particle.
   */
  double getM() const;

  /**
   * @brief Gets the type of the particle.
   * 
   * @return The type of the particle.
   */
  int getType() const;

  /**
   * @brief Overloaded equality operator.
   * 
   * @param other Another particle to compare.
   * @return True if both particles are equal, otherwise false.
   */
  bool operator==(const Particle &other) const;

  /**
   * @brief Converts the particle to a string representation.
   * 
   * @return A string representation of the particle.
   */
  std::string toString() const;
};

/**
 * @brief Overloaded stream insertion operator for particle output.
 * 
 * @param stream The output stream.
 * @param p The particle to output.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &stream, const Particle &p);