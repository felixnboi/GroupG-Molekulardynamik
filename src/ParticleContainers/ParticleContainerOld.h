#pragma once
#include "ParticleContainer.h"
 

/**
 * @class ParticleContainerOld
 * @brief A container for managing particles.
 * 
 * This class represents a container for managing particles. It provides methods to add particles
 * to the container, get the particles, as well as methods to get iterators for traversing the particles.
 */
class ParticleContainerOld : ParticleContainer{

public:
/**
 * @brief Constructor for ParticleContainerOld
*/
    ParticleContainerOld(){}

    void addParticle(const Particle& particle) override;

    const std::vector<Particle>& getParticles() override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    std::vector<std::array<Particle,2>> getParticlePairs() override;
};