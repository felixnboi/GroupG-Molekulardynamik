#pragma once
#include "../data/Particle.h"
#include "spdlog/spdlog.h"

#include <vector>
#include <iostream>

using ParticleIterator = std::vector<std::shared_ptr<Particle>>::iterator; /// Iterator type for iterating over a vector of particles.
/**
 * @brief This abstract class serves as an interface for particle containers holding particles.
 */
class ParticleContainer{
protected:
    std::vector<std::shared_ptr<Particle>> particles; ///< Vector to store the particles.
    
public:

    /**
     * @brief Virtual destructor ofr ParticleContainer
     */
    virtual ~ParticleContainer() = default;

    /**
     * @brief Returns the number of particles in this particle container.
     * 
     * @return The number of particles in this container.
     */
    virtual size_t getParticleCount() = 0;

    /**
     * @brief Reserves a memory for a known number of particles.
     * 
     * This function allows to preallocate memory for a specified number of elements.
     */
    virtual void reserve(size_t size) = 0;
        
    /**
     * @brief Add a particle to the container.
     * 
     * This function adds a particle to the container.
     * 
     * @param particle The handed over particle that needs to be added.
     */
    virtual void addParticle(const std::shared_ptr<Particle> particle) = 0;

    /**
     * @brief Get the iterator to the beginning of the particle container.
     * 
     * This function returns an iterator that is pointing to the beginning of the particle container.
     * 
     * @return An iterator to the beginning of the particle container.
     */
    virtual ParticleIterator begin() = 0;

    /**
     * @brief Get the iterator to the end of the particle container.
     * 
     * This function returns an iterator that is pointing to the end of the particle container.
     * 
     * @return An iterator to the end of the particle container.
     */
    virtual ParticleIterator end() = 0;

    /**
     * @brief Get the vector of particles.
     * 
     * This function returns a constant reference to the vector of particles.
     * 
     * @return A constant reference to the vector of particles.
     */
    virtual const std::vector<std::shared_ptr<Particle>>& getParticles() = 0;

    /**
     * @brief Returns the pairs of all particles to iterate over.
     * 
     * This function returns each pair of particles for which the force should be calculated.
     * 
     * @return A vector of the pairs of all particles to iterate over.
     */
    virtual std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairs() = 0;
};