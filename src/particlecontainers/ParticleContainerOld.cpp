#include "ParticleContainerOld.h"

ParticleContainerOld::ParticleContainerOld(){
    particle_count = 0;
    spdlog::info("Particlecontainer created.");
}

ParticleContainerOld::~ParticleContainerOld(){
    for(auto particle: particles){
        delete particle;
    }
    spdlog::info("Particlecontainer destructed.");
}

size_t ParticleContainerOld::getParticleCount(){
    return particle_count;
}

void ParticleContainerOld::reserve(size_t size){
    particles.reserve(size);
}

void ParticleContainerOld::addParticle(Particle* particle) {
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

const std::vector<Particle*>& ParticleContainerOld::getParticles() {
    spdlog::debug("Retrieved particles from the container");
    return particles;
}

std::vector<std::array<Particle*,2>> ParticleContainerOld::getParticlePairs(){
    std::vector<std::array<Particle*,2>> particlePairs;
    for (auto particle_i = particles.begin(); particle_i != particles.end(); particle_i++){
        for (auto particle_j = std::next(particle_i); particle_j!=particles.end(); particle_j++){
            particlePairs.push_back({*particle_i, *particle_j});
        }
    }
    return particlePairs;
}

void ParticleContainerOld::makeMembrane(int sizeX, int sizeY){
    for(int x = 0; x < sizeX; x++){
        for(int y = 0; y < sizeY; y++){
            int position = x+y*sizeX;
            auto particle = particles[position];

            // for every particle we check if it sill has the coresponfing neighbour and if yes we add it
            if(x +1 < sizeX) particle->addNeighbour(particles[position+1], 0);
            if(y+1 < sizeY){
                if(x -1 >= 0) particle->addNeighbour(particles[position+sizeX-1], 1);
                particle->addNeighbour(particles[position+sizeX], 2);
                if(x +1 < sizeX) particle->addNeighbour(particles[position+sizeX+1], 1);
            }
        }
    }
}

void ParticleContainerOld::applyForce(int x, int y, int sizeX, std::array<double, 3> force){
    particles[x+y*sizeX]->applyF(force);
}