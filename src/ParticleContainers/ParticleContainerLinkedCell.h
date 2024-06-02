#pragma once
#include "ParticleContainer.h"
 

class ParticleContainerLinkedCell : public ParticleContainer{

private:
    std::vector<Particle>* linkedCells;
    size_t cellSizeX;
    size_t cellSizeY;
    size_t cellSizeZ;
    size_t radius;
    std::vector<Particle> halo;
    size_t arraylenght;
public:
/**
 * @brief Constructor for ParticleContainerLinkedCell
*/
    ParticleContainerLinkedCell(size_t sizeX, size_t sizeY, size_t sizeZ, size_t radius);

    ~ParticleContainerLinkedCell();

    void reserve(size_t size) override;

    void addParticle(const Particle& particle) override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    const std::vector<Particle>& getParticles() override;

    std::vector<std::array<Particle,2>> getParticlePairs() override;

    std::vector<Particle> getBoundary();
};