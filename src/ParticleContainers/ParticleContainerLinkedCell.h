#pragma once
#include "ParticleContainer.h"
 

class ParticleContainerLinkedCell : public ParticleContainer{

private:
    std::vector<Particle>* linkedCells;
    std::array<size_t, 3> size;
    std::array<size_t, 3>  cellSize;
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

    void updateLoctions(std::array<bool,6> outflowflag);

    const std::vector<Particle>& getParticles() override;

    const std::array<size_t, 3> getSize();

    const std::array<size_t, 3> getCellSize();

    std::vector<std::array<Particle,2>> getParticlePairs() override;

    std::vector<Particle> getBoundary();
};