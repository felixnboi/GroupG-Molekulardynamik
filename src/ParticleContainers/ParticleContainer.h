#pragma once
#include "Particle.h"
#include <vector>
#include <iostream>

using ParticleIterator = std::vector<Particle>::iterator; /// Iterator type for iterating over a vector of particles.

class ParticleContainer
{
    protected:
        std::vector<Particle> particles; ///< Vector to store the particles.
    public:

    /**
     * @brief Returns the pairs of all particles to iterate over.
     * 
     * This function returns each pair of particles for which the force should be calculated.
     * 
     * @return A vector of the pairs of all particles to iterate over.
     */
        virtual std::vector<std::array<Particle,2>> getParticlePairs();

            
    /**
     * @brief Add a particle to the container.
     * 
     * This function adds a particle to the container.
     * 
     * @param particle The handed over particle that needs to be added.
     */
        virtual void addParticle(const Particle& particle);

    /**
     * @brief Get the vector of particles.
     * 
     * This function returns a constant reference to the vector of particles.
     * 
     * @return A constant reference to the vector of particles.
     */
    virtual const std::vector<Particle>& getParticles();

    /**
     * @brief Get the iterator to the beginning of the particle container.
     * 
     * This function returns an iterator that is pointing to the beginning of the particle container.
     * 
     * @return An iterator to the beginning of the particle container.
     */
        virtual ParticleIterator begin();

    /**
     * @brief Get the iterator to the end of the particle container.
     * 
     * This function returns an iterator that is pointing to the end of the particle container.
     * 
     * @return An iterator to the end of the particle container.
     */
        virtual ParticleIterator end();
};