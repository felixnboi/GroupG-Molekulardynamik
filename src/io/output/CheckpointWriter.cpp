#include "CheckpointWriter.h"

void CheckpointWriter::writeCheckpoint(ParticleContainer &particles,  const char *filename){
    std::fstream checkpoint_file;
    std::string tmp_string;
    checkpoint_file.open(filename,std::ios::in|std::ios::out|std::ios::trunc);
    int count = 0;
    for(auto p : particles.getParticles()) {
        count++;
    }
    checkpoint_file << count << "\n";
    for(auto p : particles.getParticles()){
        auto x = p->getX()+p->getDomainStart();
        auto v = p->getV();
        auto f = p->getF();
        auto oldF = p->getOldF();
        auto type = p->getType();
        checkpoint_file << x[0] << " " << x[1] << " " << x[2] << " " << v[0] << " " << v[1] << " " << v[2]<< " " << p->getM() << 
            " " << p->getEpsilon() << " "<< p->getSigma() << " "<< type <<" " << p->getIsOuter() <<  " " <<
            f[0] << " " << f[1] << " " << f[2] << " " << oldF[0] << " " << oldF[1] << " " << oldF[2]<< "\n";
        // checkpoint_file << x[0] << " " << x[1] << " " << x[2] << " " << v[0] << " " << v[1] << " " << v[2]<< " " << p->getM() << 
        //     " " << p->getIsOuter() << " "<< type <<" " << p->getEpsilon() << " "<< p->getSigma() << " " <<
        //     f[0] << " " << f[1] << " " << f[2] << " " << oldF[0] << " " << oldF[1] << " " << oldF[2]<< "\n";           
    }
    checkpoint_file.close();
}