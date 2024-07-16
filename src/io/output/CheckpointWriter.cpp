#include "CheckpointWriter.h"

void CheckpointWriter::writeCheckpoint(ParticleContainer &particles,  const char *filename){
    std::fstream checkpoint_file;
    std::string tmp_string;
    checkpoint_file.open(filename,std::ios::in|std::ios::out|std::ios::trunc);
    const int count = particles.getParticles().size();
    checkpoint_file << count << "\n";
    const auto& particle_list = particles.getParticles();

    for(const auto& p : particle_list){
       const auto& x = p->getX()+p->getDomainStart();
       const auto& v = p->getV();
       const auto& f = p->getF();
       const auto& oldF = p->getOldF();
       const auto type = p->getType();
        checkpoint_file << x[0] << " " << x[1] << " " << x[2] << " " << v[0] << " " << v[1] << " " << v[2]<< " " << p->getM() << 
            " " << p->getEpsilon() << " "<< p->getSigma() << " "<< type <<" " << p->getIsOuter() <<  " " <<
            f[0] << " " << f[1] << " " << f[2] << " " << oldF[0] << " " << oldF[1] << " " << oldF[2]<< "\n";     
    }
    checkpoint_file.close();
}