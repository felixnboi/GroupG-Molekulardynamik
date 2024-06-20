#include "CheckpointWriter.h"

void CheckpointWriter::writeCheckpoint(ParticleContainer &particles){
    std::fstream checkpoint_file;
    checkpoint_file.open("../../../input/checkpoint.txt",std::ios::out | std::ios::trunc);
    for(auto p : particles.getParticles()){
        auto x = p->getX()+p->getDomainStart();
        auto v = p->getV();
        auto f = p->getF();
        auto oldF = p->getOldF();
        auto type = p->getType();
        checkpoint_file << x[0] << " " << x[1] << " " << x[2] << " " << v[0] << " " << v[1] << " " << v[2]<< " " << p->getM() << 
            " " << p->getEpsilon() << " "<< p->getSigma() << " "<< type << " " <<
            f[0] << " " << f[1] << " " << f[2] << " " << oldF[0] << " " << oldF[1] << " " << oldF[2]<< "\n";         
    }
}