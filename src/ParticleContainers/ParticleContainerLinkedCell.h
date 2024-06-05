#pragma once
#include "ParticleContainer.h"
 

class ParticleContainerLinkedCell : public ParticleContainer{

private:
    std::unique_ptr<std::list<std::shared_ptr<Particle>>[]> linkedCells;
    std::array<double, 3> size;
    std::array<double, 3> cellSize;
    std::array<size_t, 3> cellCount;
    double radius;
    std::vector<std::shared_ptr<Particle>> halo;
    size_t arraylenght;
public:
/**
 * @brief Constructor for ParticleContainerLinkedCell
*/
    ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius);

    ~ParticleContainerLinkedCell();

    void reserve(size_t size) override;

    void addParticle(const std::shared_ptr<Particle> particle) override;

    ParticleIterator begin() override;

    ParticleIterator end() override;

    void updateLoctions(std::array<bool,6> outflowflag);

    const std::vector<std::shared_ptr<Particle>>& getParticles() override;

    const std::array<double, 3> getSize();

    const std::array<double, 3> getCellSize();

    std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairs() override;

    std::vector<std::shared_ptr<Particle>> getBoundary();
};