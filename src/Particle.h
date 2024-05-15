/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

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
   * @param x_arg Initial position of the particle.
   * @param v_arg Initial velocity of the particle.
   * @param m_arg Mass of the particle.
   * @param type_arg Type of the particle.
   */
  Particle(
      // for visualization, we need always 3 coordinates
      // -> in case of 2d, we use only the first and the second
      std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
      int type_arg = 0);

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
  bool operator==(const Particle &other);

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
