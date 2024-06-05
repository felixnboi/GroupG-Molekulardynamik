#include "ParticleContainerLinkedCell.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius){
    size = {sizeX, sizeY, sizeZ};
    cellCount = {(size_t)ceil(sizeX/radius), (size_t)ceil(sizeY/radius), (size_t)ceil(sizeZ/radius)};
    cellSize = {sizeX/cellCount[0], sizeY/cellCount[1],sizeZ/cellCount[2]};
    this->radius = radius;
    arraylenght = cellCount[0]*cellCount[1]*cellCount[2];
    linkedCells = std::make_unique<std::list<std::shared_ptr<Particle>>[]>(arraylenght);
    spdlog::info("Linked cells particlecontainer created.");
}

ParticleContainerLinkedCell::~ParticleContainerLinkedCell(){
    spdlog::info("Linked cells particlecontainer destructed.");
};

void ParticleContainerLinkedCell::reserve(size_t size){
    //TODO
}

const std::array<double, 3> ParticleContainerLinkedCell::getSize()
{
    return size;
}

const std::array<double, 3> ParticleContainerLinkedCell::getCellSize(){
    return cellSize;
}

void ParticleContainerLinkedCell::addParticle(const std::shared_ptr<Particle> particle){
    particles.push_back(particle);
    size_t index = (size_t)(particle->getX()[0]/cellSize[0])+(particle->getX()[1]/cellSize[1])*cellCount[0]+(particle->getX()[2]/cellSize[2])*cellCount[0]*cellCount[1];
    linkedCells[index].push_back(particle);
}

ParticleIterator ParticleContainerLinkedCell::begin(){
    return particles.begin();
}

ParticleIterator ParticleContainerLinkedCell::end(){
    return particles.end();
}

const std::vector<std::shared_ptr<Particle>>& ParticleContainerLinkedCell::getParticles(){
    return particles;
}

std::vector<std::array<std::shared_ptr<Particle>,2>> ParticleContainerLinkedCell::getParticlePairs(){
    std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs;
    for (size_t i = 0; i < arraylenght; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            for (auto particle_j = std::next(particle_i); particle_j!=linkedCells[i].end(); particle_j++){
                particlePairs.push_back({*particle_i, *particle_j});
            }
            for (size_t j = i+1; j < arraylenght; j++){
                int distanceX = fmod(j,cellSize[0])-fmod(i,cellSize[0]);
                int distanceY = fmod(j/cellSize[0],cellSize[1])-fmod(i/cellSize[0],cellSize[1]);
                int distanceZ = j/cellSize[0]/cellSize[1]-i/cellSize[0]/cellSize[1];
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
    size_t newIndex;
    for (size_t i = 0; i < arraylenght; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            std::array<double, 3UL> cords = (*particle_i)->getX();
            for (size_t j = 0; j < 3; j++){
                if(cords[j]<0) {
                    if(outflowflag[2*j]){
                        halo.push_back(*particle_i);
                        particle_i = --linkedCells[i].erase(particle_i);
                        goto loopend;
                    }
                    cords[j] = -cords[j];
                    std::array<double, 3UL> speed = (*particle_i)->getV();
                    speed[j] *= -1;
                    (*particle_i)->setV(speed);
                }
                if(cords[j]>size[j]) {
                    if(outflowflag[2*j+1]){
                        halo.push_back(*particle_i);
                        particle_i = --linkedCells[i].erase(particle_i);
                        goto loopend;
                    }
                    cords[j] = fmod(cords[j],(2*size[j]));
                    if(cords[j]>size[j]){
                        cords[j] = 2*size[j]-cords[j];
                    }
                }
                (*particle_i)->setX(cords);
            }
            newIndex = (size_t)((*particle_i)->getX()[0]/cellSize[0])+(size_t)((*particle_i)->getX()[1]/cellSize[1])*cellCount[0]+(size_t)((*particle_i)->getX()[2]/cellSize[2])*cellCount[0]*cellCount[1];
            if(newIndex != i){
                linkedCells[newIndex].push_back(*particle_i);
                particle_i = --linkedCells[i].erase(particle_i);
            }
            loopend: ;
        }
    }
}

std::vector<std::shared_ptr<Particle>> ParticleContainerLinkedCell::getBoundary(){
    std::vector<std::shared_ptr<Particle>> boundary;
    for (size_t i = 0; i < arraylenght; i++){
        size_t positionX = i%cellCount[0];
        size_t positionY = ((size_t)(i/cellCount[0]))%cellCount[1];
        size_t positionZ = ((size_t)(i/cellCount[0]))/cellCount[1];
        if(positionX%(cellCount[0]-1)*(positionY%(cellCount[1]-1))*(positionZ%(cellCount[2]-1))==0){
            for(auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
                boundary.push_back(*particle_i);
            }
        }
    }
    return boundary;
}

