#include "ParticleContainerLinkedCell.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius){
    particle_count = 0;
    cellCount = {(size_t)floor(sizeX/radius), (size_t)floor(sizeY/radius), (size_t)floor(sizeZ/radius)};
    if(cellCount[0] == 0) cellCount[0] = 1;
    if(cellCount[1] == 0) cellCount[1] = 1;
    if(cellCount[2] == 0) cellCount[2] = 1;
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

size_t ParticleContainerLinkedCell::getParticleCount(){
    return particle_count;
}

void ParticleContainerLinkedCell::reserve(size_t size){
    particles.reserve(size);
}

const std::array<double, 3> ParticleContainerLinkedCell::getSize(){
    return size;
}

const std::array<double, 3> ParticleContainerLinkedCell::getCellSize(){
    return cellSize;
}

void ParticleContainerLinkedCell::addParticle(const std::shared_ptr<Particle> particle){
    particle_count++;
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
    return getParticlePairsPeriodic({false,false,false});
}

std::vector<std::array<std::shared_ptr<Particle>,2>> ParticleContainerLinkedCell::getParticlePairsPeriodic(std::array<bool, 3> pFlag){
    std::vector<std::array<std::shared_ptr<Particle>,2>> particlePairs;
    particlePairs.reserve(particle_count);
    for (size_t i = 0; i < arrayLength; i++){
        std::array<size_t,13> nbrs; // array of the neighbour cells of the current cell. (Conatins only half of them so that each pair is not considered twice, 13 because (3^3-1)/2 = 13)
        size_t nbrCount = 0; // cout of how many elements are in the nbrs array (how many neighbour cells this one has, while ignoring half of them)
        std::array<size_t,3> indices = {i%cellCount[0],i/cellCount[0]%cellCount[1], i/cellCount[0]/cellCount[1]}; // indices of the current cell if it was stored in a 3 dimesional array
        std::array<int,5> nbrIndices = {(int)indices[0]-1,(int)indices[0]+1, (int)indices[1]-1, (int)indices[1]+1, (int)indices[2]+1}; // indices of the neighbours if they were stored
        // in a 3 dimesional array, the first is for negative x the second for positive, 3 and 4 are the same for y and 5 is for positive z (no negative z since we only look at half the cells)
        
        // if the pFlag (peridic flag) for a dimesion is set, the cell count is added or substracted to the neighbour indices, since we are only interseted in cells that are conected through
        // these bounderies and invalid indices are filtered out later
        if(pFlag[0]){
            nbrIndices[0] += cellCount[0];
            nbrIndices[1] -= cellCount[0];
        }
        if(pFlag[1]){
            nbrIndices[2] += cellCount[1];
            nbrIndices[3] -= cellCount[1];
        }
        if(pFlag[2]){
            nbrIndices[4] -= cellCount[2];
        }

        // array that cecks if the indices of nbrIndices are valid (in the domain)
        std::array<bool, 5> nbrExists = {nbrIndices[0]>=0 && nbrIndices[0] < (int)cellCount[0], 
        nbrIndices[1]<(int)cellCount[0] && nbrIndices[1] >= 0, nbrIndices[2]>=0 && nbrIndices[2]<(int)cellCount[1], 
        nbrIndices[3]>=0 && nbrIndices[3]<(int)cellCount[1], nbrIndices[4]>=0 && nbrIndices[4]<(int)cellCount[2]};

        // the indices are changed, so that they now represet their indices in a one dimesional array
        indices[1] *= cellCount[0];
        indices[2] *= cellCount[0]*cellCount[1];
        nbrIndices[2] *= cellCount[0];
        nbrIndices[3] *= cellCount[0];
        nbrIndices[4] *= cellCount[0]*cellCount[1];

        // for all 13 possible neighbours we want to cosider, we test if they exist. We also filter them out if they don't need a perdic boundery which is set true.
        // If they are valid we callculate their index in the one dimesional array by adding the already updated indices of all three dimesions. 
        if(!pFlag[1]&&!pFlag[2]&&nbrExists[1]                            ) nbrs[nbrCount++] = nbrIndices[1]+indices[1]   +indices[2]   ; 
        if(           !pFlag[2]&&nbrExists[0]&&nbrExists[3]              ) nbrs[nbrCount++] = nbrIndices[0]+nbrIndices[3]+indices[2]   ; 
        if(!pFlag[0]&&!pFlag[2]&&              nbrExists[3]              ) nbrs[nbrCount++] = indices[0]   +nbrIndices[3]+indices[2]   ; 
        if(           !pFlag[2]&&nbrExists[1]&&nbrExists[3]              ) nbrs[nbrCount++] = nbrIndices[1]+nbrIndices[3]+indices[2]   ; 
        if(                      nbrExists[0]&&nbrExists[2]&&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[0]+nbrIndices[2]+nbrIndices[4]; 
        if(!pFlag[0]                         &&nbrExists[2]&&nbrExists[4]) nbrs[nbrCount++] = indices[0]   +nbrIndices[2]+nbrIndices[4]; 
        if(                      nbrExists[1]&&nbrExists[2]&&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[1]+nbrIndices[2]+nbrIndices[4]; 
        if(!pFlag[1]           &&nbrExists[0]              &&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[0]+indices[1]   +nbrIndices[4]; 
        if(!pFlag[0]&&!pFlag[1]                            &&nbrExists[4]) nbrs[nbrCount++] = indices[0]   +indices[1]   +nbrIndices[4]; 
        if(!pFlag[1]           &&nbrExists[1]              &&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[1]+indices[1]   +nbrIndices[4]; 
        if(                      nbrExists[0]&&nbrExists[3]&&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[0]+nbrIndices[3]+nbrIndices[4]; 
        if(!pFlag[0]                         &&nbrExists[3]&&nbrExists[4]) nbrs[nbrCount++] = indices[0]   +nbrIndices[3]+nbrIndices[4]; 
        if(                      nbrExists[1]&&nbrExists[3]&&nbrExists[4]) nbrs[nbrCount++] = nbrIndices[1]+nbrIndices[3]+nbrIndices[4]; 

        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            if(!(pFlag[0]||pFlag[1]||pFlag[2])){
                // here we look at the particles in the same cell
                for (auto particle_j = std::next(particle_i); particle_j!=linkedCells[i].end(); particle_j++){
                    if(ArrayUtils::L2Norm((*particle_i)->getX()-(*particle_j)->getX())<radius){
                        particlePairs.push_back({*particle_i, *particle_j});
                    }
                }
            }
            for (size_t j = 0; j < nbrCount; j++){
                // here we look at the particles in the neighbour cells
                for (auto particle_j = linkedCells[nbrs[j]].begin(); particle_j != linkedCells[nbrs[j]].end(); particle_j++){
                    if(pFlag[0]||pFlag[1]||pFlag[2]||ArrayUtils::L2Norm((*particle_i)->getX()-(*particle_j)->getX())<radius){
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
    
const std::array<size_t, 3> ParticleContainerLinkedCell::getCellCount(){
    return cellCount;
}

const double ParticleContainerLinkedCell::getRadius(){
    return radius;
}

void ParticleContainerLinkedCell::updateLoctions(std::array<bool,6> outflowflag, std::array<bool,3> peridicflag){
    size_t newIndex;
    for (size_t i = 0; i < arrayLength; i++){
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            std::array<double, 3UL> cords = (*particle_i)->getX();
            for (size_t j = 0; j < 3; j++){
                if(peridicflag[j]){
                    cords[j] = fmod(cords[j],(size[j]));
                    if(cords[j]<0) cords[j]+=size[j];
                }
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

void ParticleContainerLinkedCell::makeMembrane(int sizeX, int sizeY){
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

void ParticleContainerLinkedCell::applyForce(int x, int y, int sizeX, std::array<double, 3> force){
    particles[x+y*sizeX]->applyF(force);
}
