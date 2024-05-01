//This is our Particlecontainer as stated in task 5

#pragma once

#include <vector>
#include "Particle.h"

using ParticleIterator = std::vector<Particle>::iterator;


class ParticleContainer {
private:
    std::vector<Particle> particles;

public:
    void addParticle(const Particle& particle);

    const std::vector<Particle>& getParticles();

    ParticleIterator beginParticles();

    ParticleIterator endParticles();

};