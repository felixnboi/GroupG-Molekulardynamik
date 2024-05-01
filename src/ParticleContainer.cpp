
#include "ParticleContainer.h"

// Add a particle to the container
void ParticleContainer::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

// Getters for particles
const std::vector<Particle>& ParticleContainer::getParticles() {
    return particles;
}

ParticleIterator ParticleContainer::beginParticles() {
    return particles.begin();
}

ParticleIterator ParticleContainer::endParticles() {
    return particles.end();
}
