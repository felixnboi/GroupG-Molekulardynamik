#include "ParticleContainerOld.h"

ParticleContainerOld::ParticleContainerOld(){
    particle_count = 0;
    spdlog::info("Particlecontainer created.");
}

ParticleContainerOld::~ParticleContainerOld(){
    spdlog::info("Particlecontainer destructed.");
}

size_t ParticleContainerOld::getParticleCount(){
    return particle_count;
}

void ParticleContainerOld::reserve(size_t size){
    particles.reserve(size);
}

void ParticleContainerOld::addParticle(const std::shared_ptr<Particle> particle) {
    particle_count++;
    particles.push_back(particle);
    spdlog::debug("Added a particle to the container");
    spdlog::trace("A particle with type {}", (*particle).getType());
}

ParticleIterator ParticleContainerOld::begin(){
    spdlog::debug("Returned iterator to the beginning of particles");
    return particles.begin();
}

ParticleIterator ParticleContainerOld::end() {
    spdlog::debug("Returned iterator to the end of particles");
    return particles.end();
}

const std::vector<std::shared_ptr<Particle>>& ParticleContainerOld::getParticles() {
    spdlog::debug("Retrieved particles from the container");
    return particles;
}

std::vector<std::array<std::shared_ptr<Particle>,2>> ParticleContainerOld::getParticlePairs(){
    std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs;
    for (auto particle_i = particles.begin(); particle_i != particles.end(); particle_i++){
        for (auto particle_j = std::next(particle_i); particle_j!=particles.end(); particle_j++){
            particlePairs.push_back({*particle_i, *particle_j});
        }
    }
    return particlePairs;
}