#pragma once
#include "ParticleContainer.h"
 

/**
 * @class ParticleContainerOld
 * @brief A container for managing particles.
 * 
 * This class represents a container for managing particles. It provides methods to add particles
 * to the container, get the particles, as well as methods to get iterators for traversing the particles.
 */
class ParticleContainerOld : public ParticleContainer{

public:
/**
 * @brief Constructor for ParticleContainerOld
*/
    ParticleContainerOld();
    
    ~ParticleContainerOld();

    void reserve(size_t size) override;

    void addParticle(const std::shared_ptr<Particle> particle) override;
    
    ParticleIterator begin() override;

    ParticleIterator end() override;

    const std::vector<std::shared_ptr<Particle>>& getParticles() override;

    std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairs() override;
};