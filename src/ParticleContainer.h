#pragma once

#include "Particle.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <vector>

using ParticleIterator = std::vector<Particle>::iterator; /// Iterator type for iterating over a vector of particles.

/**
 * @class ParticleContainer
 * @brief A container for managing particles.
 * 
 * This class represents a container for managing particles. It provides methods to add particles
 * to the container, get the particles, as well as methods to get iterators for traversing the particles.
 */
class ParticleContainer {
private:
    std::vector<Particle> particles; ///< Vector to store the particles.

public:
/**
 * @brief Constructor for ParticleContainer
*/
    ParticleContainer(){}

/**
 * @brief Add a particle to the container.
 * 
 * This function adds a particle to the container.
 * 
 * @param particle The handed over particle that needs to be added.
 */
    void addParticle(const Particle& particle);

/**
 * @brief Get the vector of particles.
 * 
 * This function returns a constant reference to the vector of particles.
 * 
 * @return A constant reference to the vector of particles.
 */
    const std::vector<Particle>& getParticles();

/**
 * @brief Get the iterator to the beginning of the particle container.
 * 
 * This function returns an iterator that is pointing to the beginning of the particle container.
 * 
 * @return An iterator to the beginning of the particle container.
 */
    ParticleIterator begin();

/**
 * @brief Get the iterator to the end of the particle container.
 * 
 * This function returns an iterator that is pointing to the end of the particle container.
 * 
 * @return An iterator to the end of the particle container.
 */
    ParticleIterator end();

/**
 * @brief Reserves a memory for a known number of particles.
 * 
 * This function allows to preallocate memory for a specified number of elements.
 */
    void reserve(size_t n);
};