#pragma once
#include "ParticleContainer.h"
 

/**
 * @brief A container for managing particles.
 * 
 * This class represents a container for managing particles. It provides methods to add particles
 * to the container, get the particles, as well as methods to get iterators for traversing the particles.
 */
class ParticleContainerOld : public ParticleContainer{
public:
    /**
     * @brief Constructor for ParticleContainerOld.
    */
    ParticleContainerOld();

    /**
    * @brief Destructor for ParticleContainerOld.
    */
    ~ParticleContainerOld();

    /**
     * @brief Returns the number of particles in this particle container.
     * 
     * @return The number of particles in this container.
     */
    size_t getParticleCount() override;

    /**
     * @brief Reserves space for a given number of particles.
     * 
     * @param size The number of particles to reserve space for.
     */
    void reserve(size_t size) override;

    /**
     * @brief Adds a particle to the container.
     * 
     * @param particle The particle to add.
     */
    void addParticle(const std::shared_ptr<Particle> particle) override;

    /**
     * @brief Returns an iterator to the beginning of the particles.
     * 
     * @return An iterator to the beginning of the particles.
     */
    ParticleIterator begin() override;

    /**
     * @brief Returns an iterator to the end of the particles.
     * 
     * @return An iterator to the end of the particles.
     */
    ParticleIterator end() override;

    /**
     * @brief Gets the particles in the container.
     * 
     * @return A reference to the vector of particles.
     */
    const std::vector<std::shared_ptr<Particle>>& getParticles() override;
    
    /**
     * @brief Gets all possible pairs of particles.
     * 
     * @return A vector of pairs of particles.
     */
    std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairs() override;

private:
    size_t particle_count; ///< The number of particles in this container.
};