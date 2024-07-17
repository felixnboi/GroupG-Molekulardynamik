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
#include <stddef.h>
#include <string>
#include <iostream>
#include <omp.h>


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
  const double m;

  /**
   * Type of the particle. Use it for whatever you want (e.g. to separate
   * molecules belonging to different bodies, matters, and so on)
   */
  const int type;

  bool is_outer;
  double sigma; ///< The sigma value for the Lennard-Jones potential.  
  double sigmaHalf; ///< The sigma divided by two.
  double epsilon; ///< The epsilon value for the Lennard-Jones potential.
  double rootEpsilon; ///< The root of epsilon.
  const std::array<double, 3> domainStart; ///< The domain start position of the particle.
  size_t strategy; ///< The strategy we use for parallellization.
  
  /**
   * Half of the neighbour particles in the membrane. (Specificly the ones below and the one to the right).
   */
  std::array<Particle*,4> neighbours;

  /**
   * Array to check if the neighbours actually exist.
   */
  std::array<bool,4> hasNeighbour {};

  omp_lock_t lock; ///< OpenMP lock.

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
   * @param type_arg The type of the particle.
   * @param is_outer_arg Whether the particle belongs to the outer cuboid (wall).
   * @param sigma The sigma value for the Lennard-Jones potential.
   * @param epsilon The epsilon value for the Lennard-Jones potential.
   * @param domainStart The domain start position of the particle.
   */
  Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, int type_arg, bool is_outer_arg, 
  double sigma, double epsilon, std::array<double,3> domainStart);

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
   * @brief Gets the domain start coordinates.
   * 
   * @return Reference to the coordinates array.
   */
  const std::array<double, 3> &getDomainStart() const;

  /**
   * @brief Gets the half of the neighbours of this particle in the membrane.
   * 
   * @return Reference to the neighbours array.
   */
  const std::array<Particle*,4> &getNeighbours() const;

  /**
   * @brief Gets a bool array which says if the neighbours actually exist.
   * 
   * @return Reference to the hasNeighbours array.
   */
  const std::array<bool,4> &getHasNeighbour() const;

   /**
   * @brief Gets the is_outer parameter of the particle.
   * 
   * @return Boolean value of is_outer.
   */
  const bool getIsOuter() const;

  /**
   * @brief Gets the value of sigma for this particle.
   * 
   * @return The value of sigma.
   */
  const double getSigma() const;

  /**
   * @brief Gets the value of sigmaHalf for this particle.
   * 
   * @return The value of sigmaHalf.
   */
  const double getSigmaHalf() const;

  /**
   * @brief Gets the value of epsilon for this particle.
   * 
   * @return The value of epsilon.
   */
  const double getEpsilon() const;

  /**
   * @brief Gets the value of rootEpsilon for this particle.
   * 
   * @return The value of rootEpsilon.
   */
  const double getRootEpsilon() const;

  /**
   * @brief Gets the mass of the particle.
   * 
   * @return The mass of the particle.
   */
  const double getM() const;

  /**
   * @brief Gets the type of the particle.
   * 
   * @return The type of the particle.
   */
  const int getType() const;

  /**
   * @brief Sets the position of the particle.
   * 
   * @param newX New position array.
   */
  void setX(const std::array<double, 3>& newX);

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

  /**
   * @brief Sets the old force on this particle.
   * 
   * @param newOldF New old force array.
   */
  void setOldF(const std::array<double, 3>& newOldF);
  
  /**
   * @brief This function adds the specified force on top.
   * 
   * @param force The force vector to be added on top.
   */
  void applyF(const std::array<double, 3>& force);

  /**
   * @brief Sets the is_outer parameter of the particle.
   * 
   * @param newIsOuter New is_outer value.
   */
  void setIsOuter(const bool newIsOuter);

  /**
   * @brief Sets the strategy parameter of the particle.
   * 
   * @param newStrategy New strategy value.
   */
  void setStrategy(const size_t newStrategy);

  /**
   * @brief Adds a neighbour of this particle in the membrane.
   * 
   * @param neighbour The neighbour that is to be added.
   * @param position The position in the neighbour array.
   */
  void addNeighbour(Particle* neighbour, const int position);

  /**
   * @brief Converts the particle to a string representation.
   * 
   * @return A string representation of the particle.
   */
  std::string toString() const;

  /**
   * @brief Overloaded equality operator.
   * 
   * @param other Another particle to compare.
   * @return True if both particles are equal, otherwise false.
   */
  bool operator==(const Particle &other) const;
};

/**
 * @brief Overloaded stream insertion operator for particle output.
 * 
 * @param stream The output stream.
 * @param p The particle to output.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &stream, const Particle &p);