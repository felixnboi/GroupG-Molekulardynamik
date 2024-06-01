/**
 * @file ParticleContainerOld.cpp
 * @brief This file contains a ParticleContainerOld class and its functions.
 */
#include "ParticleContainerOld.h"
#include "spdlog/spdlog.h"

void ParticleContainerOld::addParticle(const Particle& particle) {
    particles.push_back(particle);
    spdlog::debug("Added a particle to the container");
    spdlog::trace("A particle with type {}", particle.getType());
}

const std::vector<Particle>& ParticleContainerOld::getParticles() {
    return particles;
    spdlog::debug("Retrieved particles from the container");
}

std::vector<std::array<Particle,2>> ParticleContainerOld::getParticlePairs(){
    std::vector<std::array<Particle,2>> particlePairs;
    for (auto particle_i = particles.begin(); particle_i != particles.end(); particle_i++){
        for (auto particle_j = std::next(particle_i); particle_j!=particles.end(); particle_j++){
            particlePairs.push_back({*particle_i, *particle_j});
        }
    }
    return particlePairs;
}

ParticleIterator ParticleContainerOld::begin(){
    return particles.begin();
    spdlog::debug("Returned iterator to the beginning of particles");
}

ParticleIterator ParticleContainerOld::end() {
    return particles.end();
    spdlog::debug("Returned iterator to the end of particles");
}