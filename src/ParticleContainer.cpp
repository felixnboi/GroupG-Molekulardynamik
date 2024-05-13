
/**
 * @file ParticleContainer.cpp
 * @brief This file contains a ParticleContainer class and its functions.
 */
#include "ParticleContainer.h"


void ParticleContainer::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

const std::vector<Particle>& ParticleContainer::getParticles() {
    return particles;
}

ParticleIterator ParticleContainer::begin() {
    return particles.begin();
}

ParticleIterator ParticleContainer::end() {
    return particles.end();
}
