#include "ParticleContainerLinkedCell.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius){
    cellCount = {(size_t)ceil(sizeX/radius), (size_t)ceil(sizeY/radius), (size_t)ceil(sizeZ/radius)};
    cellSize = {sizeX/cellCount[0], sizeY/cellCount[1],sizeZ/cellCount[2]};
    size = {cellSize[0]*cellCount[0],cellSize[1]*cellCount[1],cellSize[2]*cellCount[2]};
    this->radius = radius;
    arrayLength = cellCount[0]*cellCount[1]*cellCount[2];
    linkedCells = std::make_unique<std::list<std::shared_ptr<Particle>>[]>(arrayLength);
    spdlog::info("Linked cells particlecontainer created.");
}

ParticleContainerLinkedCell::~ParticleContainerLinkedCell(){
    spdlog::info("Linked cells particlecontainer destructed.");
};

void ParticleContainerLinkedCell::reserve(size_t size){
    particles.reserve(size);
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
    auto cords = particle->getX();
    if(cords[0]<0||cords[0]>=size[0]||cords[1]<0||cords[1]>=size[1]||cords[2]<0||cords[2]>=size[2]){
        halo.push_back(particle);
        spdlog::info("Added Particle is not inside of the calculated area\n");
    }
    else{
        size_t index = (size_t)(particle->getX()[0]/cellSize[0])+((size_t)(particle->getX()[1]/cellSize[1]))*cellCount[0]+((size_t)(particle->getX()[2]/cellSize[2]))*cellCount[0]*cellCount[1];
        linkedCells[index].push_back(particle);
    }
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
    for (size_t i = 0; i < arrayLength; i++){
        size_t nbrCount = 0;
        std::array<size_t,13> nbrs;
        std::array<size_t,3> indexes = {i%cellCount[0],i/cellCount[0]%cellCount[1], i/cellCount[0]/cellCount[1]};
        bool nbrExists[5] = {indexes[0]>0, indexes[0]+1<cellCount[0], indexes[1]>0, indexes[1]+1<cellCount[1], indexes[2]+1<cellCount[2]};

        if(                            nbrExists[1]) nbrs[nbrCount++] = i                                       +1;
        if(              nbrExists[3]&&nbrExists[0]) nbrs[nbrCount++] = i                          +cellCount[0]-1;
        if(              nbrExists[3]              ) nbrs[nbrCount++] = i                          +cellCount[0]  ;
        if(              nbrExists[3]&&nbrExists[1]) nbrs[nbrCount++] = i                          +cellCount[0]+1;
        if(nbrExists[4]&&nbrExists[2]&&nbrExists[0]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]-cellCount[0]-1;
        if(nbrExists[4]&&nbrExists[2]              ) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]-cellCount[0]  ;
        if(nbrExists[4]&&nbrExists[2]&&nbrExists[1]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]-cellCount[0]+1;
        if(nbrExists[4]              &&nbrExists[0]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]             -1;
        if(nbrExists[4]                            ) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]               ;
        if(nbrExists[4]              &&nbrExists[1]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]             +1;
        if(nbrExists[4]&&nbrExists[3]&&nbrExists[0]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]+cellCount[0]-1;
        if(nbrExists[4]&&nbrExists[3]              ) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]+cellCount[0]  ;
        if(nbrExists[4]&&nbrExists[3]&&nbrExists[1]) nbrs[nbrCount++] = i+cellCount[0]*cellCount[1]+cellCount[0]+1;

        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            for (auto particle_j = std::next(particle_i); particle_j!=linkedCells[i].end(); particle_j++){
                if(ArrayUtils::L2Norm((*particle_i)->getX()-(*particle_j)->getX())<radius){
                    particlePairs.push_back({*particle_i, *particle_j});
                }
            }
            for (size_t j = 0; j < nbrCount; j++){
                for (auto particle_j = linkedCells[nbrs[j]].begin(); particle_j != linkedCells[nbrs[j]].end(); particle_j++){
                    if(ArrayUtils::L2Norm((*particle_i)->getX()-(*particle_j)->getX())<radius){
                        particlePairs.push_back({*particle_i, *particle_j});
                    }
                }
            }
        }
    }
    return particlePairs;
}

std::vector<std::shared_ptr<Particle>> ParticleContainerLinkedCell::getHalo(){
    return halo;
}

void ParticleContainerLinkedCell::updateLoctions(std::array<bool,6> outflowflag){
    size_t newIndex;
    for (size_t i = 0; i < arrayLength; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            std::array<double, 3UL> cords = (*particle_i)->getX();
            for (size_t j = 0; j < 3; j++){
                if(cords[j]<0) {
                    if(outflowflag[2*j]){
                        halo.push_back(*particle_i); 
                        (*particle_i)->setF({0,0,0});
                        (*particle_i)->setOldF({0,0,0});
                        particle_i = --linkedCells[i].erase(particle_i);
                        goto loopend;
                    }
                    cords[j] = -cords[j];
                    std::array<double, 3UL> speed = (*particle_i)->getV();
                    speed[j] *= -1;
                    (*particle_i)->setV(speed);
                }
                if(cords[j]>=size[j]) {
                    if(outflowflag[2*j+1]){
                        halo.push_back(*particle_i);
                       
                        particle_i = --linkedCells[i].erase(particle_i);
                        goto loopend;
                    }
                    cords[j] = fmod(cords[j],(2*size[j]));
                    if(cords[j]>size[j]){
                        std::array<double, 3UL> speed = (*particle_i)->getV();
                        speed[j] *= -1;
                        (*particle_i)->setV(speed);
                        cords[j] = 2*size[j]-cords[j];
                    }
                    if(cords[j]==size[j]) {
                        std::array<double, 3UL> speed = (*particle_i)->getV();
                        speed[j] *= -1;
                        (*particle_i)->setV(speed);
                        cords[j] = nextafter(cords[j],0);
                    }
                }
                (*particle_i)->setX(cords);
            }
            newIndex = (size_t)((*particle_i)->getX()[0]/cellSize[0])+((size_t)((*particle_i)->getX()[1]/cellSize[1]))*cellCount[0]+((size_t)((*particle_i)->getX()[2]/cellSize[2]))*cellCount[0]*cellCount[1];
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
    for (size_t i = 0; i < arrayLength; i++){
        size_t positionX = i%cellCount[0];
        size_t positionY = i/cellCount[0]%cellCount[1];
        size_t positionZ = i/cellCount[0]/cellCount[1];
        if((positionX==0)||(positionY==0)||(positionZ=0)||(positionX==cellCount[0]-1)||(positionY==cellCount[1]-1)||(positionZ==cellCount[2]-1)){
            for(auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
                boundary.push_back(*particle_i);
            }
        }
    }
    return boundary;
}

