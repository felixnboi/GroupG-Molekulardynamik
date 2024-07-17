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
    size_t getParticleCount() const override;

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
    void addParticle(Particle* particle) override;

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
    const std::vector<Particle*>& getParticles() const override;
    
    /**
     * @brief Gets all possible pairs of particles.
     * 
     * @return A vector of pairs of particles.
     */
    std::vector<std::pair<Particle* const, Particle* const>> getParticlePairs() override;

    /**
     * @brief Makes the particles now see each other as part of the same membrane.
     * 
     * @param sizeX The amount of particles of the mebrane in X dircetion.
     * @param sizeY The amount of particles of the mebrane in Y dircetion.
     */
    void makeMembrane(int sizeX, int sizeY) override;

    /**
     * @brief Applys force to the particle at the specified palce in the membrane.
     * 
     * @param sizeX The amount of particles of the mebrane in X dircetion.
     * @param x The location of the particle int the mebrane in x dircetion.
     * @param y The location of the particle int the mebrane in y dircetion.
     * @param force The force that is applyed to the particle.
     */
    void applyForce(int x, int y, int sizeX, std::array<double, 3> force) override;

private:
    size_t particle_count; ///< The number of particles in this container.
};