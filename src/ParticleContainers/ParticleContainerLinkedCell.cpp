#include "ParticleContainerLinkedCell.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(size_t sizeX, size_t sizeY, size_t sizeZ, size_t radius){
    size = {sizeX, sizeY, sizeZ};
    cellSize = {(sizeX+radius+1)/radius, (sizeY+radius+1)/radius,(sizeZ+radius+1)/radius};
    this->radius = radius;
    arraylenght = cellSize[0]*cellSize[1]*cellSize[2];
    linkedCells = new std::vector<Particle>[arraylenght];
    spdlog::info("Linked cells particlecontainer created.");
}

ParticleContainerLinkedCell::~ParticleContainerLinkedCell(){
    delete[] linkedCells;
    spdlog::info("Linked cells particlecontainer destructed.");
};

void ParticleContainerLinkedCell::reserve(size_t size){
    //TODO
}

const std::array<size_t, 3> ParticleContainerLinkedCell::getSize()
{
    return size;
}

const std::array<size_t, 3> ParticleContainerLinkedCell::getCellSize(){
    return cellSize;
}

void ParticleContainerLinkedCell::addParticle(const Particle& particle){
    particles.push_back(particle);
    size_t index = particle.getX()[0]/radius+(particle.getX()[1]/radius)*cellSize[0]+(particle.getX()[2]/radius)*cellSize[0]*cellSize[0];
    linkedCells[index].push_back(particle);
}

ParticleIterator ParticleContainerLinkedCell::begin(){
    return particles.begin();
}

ParticleIterator ParticleContainerLinkedCell::end(){
    return particles.end();
}

const std::vector<Particle>& ParticleContainerLinkedCell::getParticles(){
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
                size_t distanceX = j%cellSize[0]-i%cellSize[0];
                size_t distanceY = j/cellSize[0]%cellSize[1]-i/cellSize[0]%cellSize[1];
                size_t distanceZ = j/cellSize[0]/cellSize[1]-i/cellSize[0]/cellSize[1];
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

void ParticleContainerLinkedCell::updateLoctions(std::array<bool,6> outflowflag){
    for (size_t i = 0; i < arraylenght; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            std::array<double, 3UL> cords = particle_i->getX();
            for (size_t j = 0; j < 3; j++){
                while(cords[j]<0||cords[j]>size[j])
                {
                    if(cords[j]<0) {
                        if(outflowflag[2*j]){
                            halo.push_back(*particle_i);
                            linkedCells[i].erase(particle_i);
                            break;
                        }
                        cords[j] = -cords[j];
                    }
                    else {
                        if(outflowflag[2*j+1]){
                            halo.push_back(*particle_i);
                            linkedCells[i].erase(particle_i);
                            break;
                        }
                        cords[j] = 2*size[j]-cords[j];
                    }
                }
            }
            size_t newIndex = particle_i->getX()[0]/radius+(particle_i->getX()[1]/radius)*cellSize[0]+(particle_i->getX()[2]/radius)*cellSize[0]*cellSize[0];
            if(newIndex != i){
                linkedCells[2].push_back(*particle_i);
                linkedCells[i].erase(particle_i);
            }
        }
    }
}

std::vector<Particle> ParticleContainerLinkedCell::getBoundary(){
    std::vector<Particle> boundary;
    for (size_t i = 0; i < arraylenght; i++){
        size_t positionX = i%cellSize[0];
        size_t positionY = i/cellSize[0]%cellSize[1];
        size_t positionZ = i/cellSize[0]/cellSize[1];
        if(positionX%(cellSize[0]-1)*(positionY%(cellSize[1]-1))*(positionX%(cellSize[0]-1))){
            for(auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
                boundary.push_back(*particle_i);
            }
        }
    }
    return boundary;
}

