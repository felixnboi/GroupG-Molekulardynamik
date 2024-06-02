#pragma once
#include "ParticleContainer.h"
 

class ParticleContainerLinkedCell : ParticleContainer{

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
    ParticleContainerLinkedCell(size_t sizeX, size_t sizeY, size_t sizeZ, size_t radius){
        cellSizeX = (sizeX+radius+1)/radius;
        cellSizeY = (sizeY+radius+1)/radius;
        cellSizeZ = (sizeZ+radius+1)/radius;
        this->radius = radius;
        arraylenght = cellSizeX*cellSizeY*cellSizeZ;
        linkedCells = new std::vector<Particle>[arraylenght];
    }

    void addParticle(const Particle& particle) override;

    const std::vector<Particle>& getParticles() override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    std::vector<std::array<Particle,2>> getParticlePairs() override;
};