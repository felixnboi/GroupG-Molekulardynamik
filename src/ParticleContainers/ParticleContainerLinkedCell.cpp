#include "ParticleContainerLinkedCell.h"


void ParticleContainerLinkedCell::addParticle(const Particle& particle) {
    particles.push_back(particle);
    size_t index = particle.getX()[0]/radius+(particle.getX()[1]/radius)*cellSizeX+(particle.getX()[2]/radius)*cellSizeX*cellSizeY;
    linkedCells[index].push_back(particle);
}

const std::vector<Particle>& ParticleContainerLinkedCell::getParticles() {
    return particles;
}

std::vector<std::array<Particle,2>> ParticleContainerLinkedCell::getParticlePairs(){
    std::vector<std::array<Particle,2>> particlePairs;
    for (size_t i = 0; i < arraylenght; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            for (auto particle_j = std::next(particle_i); particle_j!=linkedCells[i].end(); particle_j++){
                particlePairs.push_back({*particle_i, *particle_j});
            }
            for (size_t j = i+1; j < arraylenght; j++){
                size_t distanceX = j%cellSizeX-i%cellSizeX;
                size_t distanceY = j/cellSizeX%cellSizeY-i/cellSizeX%cellSizeY;
                size_t distanceZ = j/cellSizeX/cellSizeY-i/cellSizeX/cellSizeY;
                if(distanceX<2&&distanceX>-2&&distanceY<2&&distanceY>-2&&distanceZ<2&&distanceZ>-2){
                    for (auto particle_j = linkedCells[j].begin(); particle_j != linkedCells[j].end(); particle_j++){
                        particlePairs.push_back({*particle_i, *particle_j});
                    }
                }
            }
        }
    }
    return particlePairs;
}

std::vector<Particle> ParticleContainerLinkedCell::getBoundery()
{
    std::vector<Particle> boundery;
    for (size_t i = 0; i < arraylenght; i++){
        size_t positionX = i%cellSizeX;
        size_t positionY = i/cellSizeX%cellSizeY;
        size_t positionZ = i/cellSizeX/cellSizeY;
        if(positionX%(cellSizeX-1)*(positionY%(cellSizeY-1))*(positionX%(cellSizeX-1))){
            for(auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
                boundery.push_back(*particle_i);
            }
        }
    }
}

ParticleIterator ParticleContainerLinkedCell::begin(){
    return particles.begin();
}

ParticleIterator ParticleContainerLinkedCell::end() {
    return particles.end();
}