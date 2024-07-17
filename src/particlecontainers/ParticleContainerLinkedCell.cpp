#include "ParticleContainerLinkedCell.h"

ParticleContainerLinkedCell::ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius, size_t strategy){
    this->strategy = strategy;
    particle_count = 0;
    cellCount = {(size_t)floor(sizeX/radius), (size_t)floor(sizeY/radius), (size_t)floor(sizeZ/radius)};
    if(cellCount[0] == 0) cellCount[0] = 1;
    if(cellCount[1] == 0) cellCount[1] = 1;
    if(cellCount[2] == 0) cellCount[2] = 1;
    cellSize = {sizeX/cellCount[0], sizeY/cellCount[1],sizeZ/cellCount[2]};
    size = {cellSize[0]*cellCount[0],cellSize[1]*cellCount[1],cellSize[2]*cellCount[2]};
    radiusSquared = radius*radius;
    cellCountTotal = cellCount[0]*cellCount[1]*cellCount[2];
    linkedCells = std::vector<std::vector<Particle*>>(cellCountTotal);
    lastReserve = {1,1,1,1,1,1,1,1};
    #ifdef _OPENMP
    omp_init_lock(&lock);
    #endif
    spdlog::info("Linked cells particlecontainer created.");
}

ParticleContainerLinkedCell::~ParticleContainerLinkedCell(){
    for(auto particle: particles){
        delete particle;
    }
    #ifdef _OPENMP
    omp_destroy_lock(&lock);
    #endif
    spdlog::info("Linked cells particlecontainer destructed.");
};

void ParticleContainerLinkedCell::reserve(size_t size){
    particles.reserve(size);
}

void ParticleContainerLinkedCell::addParticle(Particle* particle){
    particle_count++;
    particles.push_back(particle);
    particle->setStrategy(strategy);
    auto cords = particle->getX();
    if(cords[0]<0||cords[0]>=size[0]||cords[1]<0||cords[1]>=size[1]||cords[2]<0||cords[2]>=size[2]){
        halo.push_back(particle);
        spdlog::info("Added Particle is not inside of the calculated area\n");
    }
    else{
        linkedCells[positionToIndex(cords)].push_back(particle);
    }
    lastReserve[0] = particle_count;
}

std::vector<std::pair<Particle* const, Particle* const>> ParticleContainerLinkedCell::getParticlePairs(){
    return getParticlePairsPeriodic({false,false,false});
}

std::vector<std::pair<Particle* const, Particle* const>> ParticleContainerLinkedCell::getParticlePairsPeriodic(std::array<bool, 3> pFlag){
    std::vector<std::pair<Particle* const, Particle* const>> particlePairs; // The particle pairs we want to return.
    int index = (pFlag[0] << 2) | (pFlag[1] << 1) | pFlag[2]; // The index of last reserve.
    size_t count = 0; // The amount of particles added to particlePairs.
    particlePairs.reserve(lastReserve[index]*1.5); //We take the last reserve as our estimation, but overestimate slighty for better runtime

    // Depending on the strategy selceted the next for loop is paralellized or not. In the for loop we iterrate over all cells and for each
    // cell we call getParticlePairsPeridicOneCell, which adds all particle pairs to particlePairs, where the first particle is in the current
    // cell and also only those pairs that need all active pFlags to interact.
    if(strategy == 0 || strategy == 3){
        for (size_t i = 0; i < cellCountTotal; i++){
            count += getParticlePairsPeridicOneCell(particlePairs, i, pFlag);
        }
    }
    else { // strategy is 1 or 2
        #ifdef _OPENMP
        #pragma omp parallel for schedule(auto)
        #endif
        for (size_t i = 0; i < cellCountTotal; i++){
            size_t tmp = getParticlePairsPeridicOneCell(particlePairs, i, pFlag);
            #ifdef _OPENMP
            #pragma omp atomic
            #endif
            count += tmp;
        }
    }
    lastReserve[index] = count;
    return particlePairs;
}

inline size_t ParticleContainerLinkedCell::getParticlePairsPeridicOneCell(std::vector<std::pair<Particle* const, Particle* const>>& particlePairs, 
size_t i, std::array<bool, 3> pFlag){
    size_t count = 0; // The amount of particles added to particlePairs.
    std::array<size_t,13> nbrs; // array of the neighbour cells of the current cell. (Conatins only half of them so that each pair is not considered twice, 13 because (3^3-1)/2 = 13)
    size_t nbrCount = 0; // count of how many elements are in the nbrs array (how many neighbour cells this one has, while ignoring half of them)
    std::array<size_t,3> indices = index1DTo3D(i); // indices of the current cell if it was stored in a 3 dimesional array
    std::array<int,5> nbrIndices = {(int)indices[0]-1,(int)indices[0]+1, (int)indices[1]-1, (int)indices[1]+1, (int)indices[2]+1}; // indices of the neighbours if they were stored
    // in a 3 dimesional array, the first is for negative x the second for positive, 3 and 4 are the same for y and 5 is for positive z (no negative z since we only look at half the cells)
    
    // if the pFlag (periodic flag) for a dimesion is set, the cell count is added or substracted to the neighbour indices, since we are only interseted in cells that are conected through
    // these boundaries and invalid indices are filtered out later
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

    // Depending on the selected strategy the next for loop is parallel or not. The loop interates over all particles in the current
    // cell and for each it calls getParticlePairsPeriodicOneParticle, which adds all viable pairs with this particle to particle pairs.
    // (Or half of those because of Netwons third law) 
    if(strategy == 3){
        #ifdef _OPENMP
        #pragma omp parallel for schedule(auto)
        #endif
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            size_t tmp = getParticlePairsPeriodicOneParticle(particlePairs, i, particle_i, nbrCount, nbrs, pFlag);
            #ifdef _OPENMP
            #pragma omp atomic
            #endif
            count += tmp;
        }
    }
    else { // strategy is 0, 1 or 2
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
            count += getParticlePairsPeriodicOneParticle(particlePairs, i, particle_i, nbrCount, nbrs, pFlag);
        }
    }

    return count;
}

inline size_t ParticleContainerLinkedCell::getParticlePairsPeriodicOneParticle(std::vector<std::pair<Particle* const, Particle* const>>& particlePairs, 
size_t i, ParticleIterator particle_i, size_t nbrCount, const std::array<size_t,13>& nbrs, std::array<bool, 3> pFlag){
    size_t count = 0; // The amount of particles added to particlePairs.
    if(!(pFlag[0]||pFlag[1]||pFlag[2])){
        // here we look at the particles in the same cell
        for (auto particle_j = std::next(particle_i); particle_j!=linkedCells[i].end(); particle_j++){
            if(inCuttofRaius((*particle_i)->getX()- (*particle_j)->getX())){
                #ifdef _OPENMP
                #pragma omp critical
                #endif
                {
                    particlePairs.emplace_back(*particle_i, *particle_j);
                }
                count++;        
            }
        }
    }
    for (size_t j = 0; j < nbrCount; j++){
        // here we look at the particles in the neighbour cells
        for (auto particle_j = linkedCells[nbrs[j]].begin(); particle_j != linkedCells[nbrs[j]].end(); particle_j++){
            if(pFlag[0]||pFlag[1]||pFlag[2]||inCuttofRaius((*particle_i)->getX()- (*particle_j)->getX())){
                #ifdef _OPENMP
                #pragma omp critical
                #endif
                {
                    particlePairs.emplace_back(*particle_i, *particle_j);
                }
                count++;
            }
        }
    }
    return count;
}

inline bool ParticleContainerLinkedCell::inCuttofRaius(std::array<double, 3UL> distance){
    auto distanceSquared = distance*distance;
    return distanceSquared[0]+distanceSquared[1]+distanceSquared[2] < radiusSquared;
}

void ParticleContainerLinkedCell::updateLoctions(std::array<bool,6> outflowflag, std::array<bool,3> periodicflag){
    size_t newIndex;
    for (size_t i = 0; i < cellCountTotal; i++){ // Iterates over all cells
        for (auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){ // Iterates over all particles in this cell
            std::array<double, 3UL> cords = (*particle_i)->getX();
            for (size_t j = 0; j < 3; j++){ // Iterates over all dimensions
                if(periodicflag[j]){
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
                    //position and velocity get mirrored at the boundery
                    cords[j] = -cords[j];
                    (*particle_i)->setV(-1*(*particle_i)->getV());
                }
                if(cords[j]>=size[j]) {
                    if(outflowflag[2*j+1]){
                        halo.push_back(*particle_i);
                        (*particle_i)->setF({0,0,0});
                        (*particle_i)->setOldF({0,0,0});
                        particle_i = --linkedCells[i].erase(particle_i);
                        goto loopend;
                    }
                    cords[j] = fmod(cords[j],(2*size[j]));
                    if(cords[j]>size[j]){
                        //position and velocity get mirrored at the boundery (the fmod does the same only multible times)
                        (*particle_i)->setV(-1*(*particle_i)->getV());
                        cords[j] = 2*size[j]-cords[j];
                    }
                    if(cords[j]==size[j]) {
                        // To avoid divided by zero exeptions
                        cords[j] = nextafter(cords[j],0);
                    }
                }
                (*particle_i)->setX(cords);
            }
            newIndex = positionToIndex(cords);
            if(newIndex != i){
                linkedCells[newIndex].push_back(*particle_i);
                particle_i = --linkedCells[i].erase(particle_i);
            }
            loopend: ;
        }
    }
}

void ParticleContainerLinkedCell::makeMembrane(int sizeX, int sizeY){
    for(int x = 0; x < sizeX; x++){
        for(int y = 0; y < sizeY; y++){
            int position = x+y*sizeX;
            auto particle = particles[position];

            // for every particle we check if the coresponding neighbour does actually exist and if yes we add it
            if(x + 1 < sizeX) particle->addNeighbour(particles[position+1], 0);
            if(y + 1 < sizeY){
                if(x - 1 >= 0) particle->addNeighbour(particles[position+sizeX-1], 1);
                particle->addNeighbour(particles[position+sizeX], 2);
                if(x + 1 < sizeX) particle->addNeighbour(particles[position+sizeX+1], 1);
            }
        }
    }
}

void ParticleContainerLinkedCell::applyForce(int x, int y, int sizeX, std::array<double, 3> force){
    particles[x+y*sizeX]->applyF(force);
}

std::array<size_t, 3> ParticleContainerLinkedCell::index1DTo3D(size_t index){
    return {index%cellCount[0], index/cellCount[0]%cellCount[1], index/cellCount[0]/cellCount[1]};
}

size_t ParticleContainerLinkedCell::positionToIndex(std::array<double, 3> position){
    return (size_t)(position[0]/cellSize[0])+((size_t)(position[1]/cellSize[1]))*cellCount[0]+((size_t)(position[2]/cellSize[2]))*cellCount[0]*cellCount[1];
}


std::vector<Particle*> ParticleContainerLinkedCell::getBoundary(){
    return getBoundaries({true,true,true,true,true,true});
}

std::vector<Particle*> ParticleContainerLinkedCell::getBoundaries(std::array<bool, 6> boundaries){
    std::vector<Particle*> boundary;
    for (size_t i = 0; i < cellCountTotal; i++){
        auto indices = index1DTo3D(i);
        if((indices[0]==0&&boundaries[0])||(indices[1]==0&&boundaries[2])||(indices[2]=0&&boundaries[4])||
        (indices[0]==cellCount[0]-1&&boundaries[1])||(indices[1]==cellCount[1]-1&&boundaries[3])||
        (indices[2]==cellCount[2]-1&&boundaries[5])){
            for(auto particle_i = linkedCells[i].begin(); particle_i != linkedCells[i].end(); particle_i++){
                boundary.push_back(*particle_i);
            }
        }
    }
    return boundary;
}

std::vector<Particle*> ParticleContainerLinkedCell::getHalo(){
    return halo;
}
    
const std::array<size_t, 3> ParticleContainerLinkedCell::getCellCount(){
    return cellCount;
}

const double ParticleContainerLinkedCell::getRadiusSquared(){
    return radiusSquared;
}

const std::vector<Particle*>& ParticleContainerLinkedCell::getParticles(){
    return particles;
}

size_t ParticleContainerLinkedCell::getParticleCount(){
    return particle_count;
}

const std::array<double, 3> ParticleContainerLinkedCell::getSize(){
    return size;
}

const std::array<double, 3> ParticleContainerLinkedCell::getCellSize(){
    return cellSize;
}

ParticleIterator ParticleContainerLinkedCell::begin(){
    return particles.begin();
}

ParticleIterator ParticleContainerLinkedCell::end(){
    return particles.end();
}
