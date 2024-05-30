#include "ParticleContainer.h"

/**
 * @file ParticleContainer.cpp
 * @brief This file contains a ParticleContainer class and its functions.
 */

void ParticleContainer::allocate(size_t size){
    particles.reserve(size);
}

void ParticleContainer::addParticle(const Particle& particle) {
    particles.push_back(particle);
    spdlog::debug("Added a particle to the container");
}

const std::vector<Particle>& ParticleContainer::getParticles() {
    spdlog::debug("Retrieved particles from the container");
    return particles;
}

ParticleIterator ParticleContainer::begin(){
    spdlog::debug("Returned iterator to the beginning of particles");
    return particles.begin();
}

ParticleIterator ParticleContainer::end() {
    spdlog::debug("Returned iterator to the end of particles");
    return particles.end();
}