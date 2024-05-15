/**
 * @file ParticleContainer.cpp
 * @brief This file contains a ParticleContainer class and its functions.
 */
#include "ParticleContainer.h"
#include "spdlog/spdlog.h"

void ParticleContainer::addParticle(const Particle& particle) {
    particles.push_back(particle);
    spdlog::debug("Added a particle to the container");
    spdlog::trace("A particle with type {}", particle.getType());
}

const std::vector<Particle>& ParticleContainer::getParticles() {
    return particles;
    spdlog::debug("Retrieved particles from the container");
}

ParticleIterator ParticleContainer::beginParticles() {
    return particles.begin();
    spdlog::debug("Returned iterator to the beginning of particles");
}

ParticleIterator ParticleContainer::endParticles() {
    return particles.end();
    spdlog::debug("Returned iterator to the end of particles");
}