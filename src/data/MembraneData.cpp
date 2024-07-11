#include "MembraneData.h"

MembraneData::MembraneData() = default;

MembraneData::MembraneData(bool membrane_flag, double r0, double k, double f_z_up)
    : membrane_flag(membrane_flag), 
      r0(r0), 
      k(k), 
      f_z_up(f_z_up) {}

bool MembraneData::getMembraneFlag() const { return membrane_flag; }
double MembraneData::getR0() const { return r0; }
double MembraneData::getK() const { return k; }
double MembraneData::getF_z_up() const { return f_z_up; }
const std::vector<std::pair<size_t, size_t>>& MembraneData::getParticleUp() const { return particle_up; }

void MembraneData::setMembraneFlag(bool new_membrane_flag) { membrane_flag = new_membrane_flag; }
void MembraneData::setR0(double new_r0) { r0 = new_r0; }
void MembraneData::setK(double new_k) { k = new_k; }
void MembraneData::setF_z_up(double new_f_z_up) { f_z_up = new_f_z_up; }
void MembraneData::setParticleUp(const std::vector<std::pair<size_t, size_t>>& new_particle_up) { particle_up = new_particle_up; }

void MembraneData::addParticleUp(size_t x_index, size_t y_index) {
  particle_up.emplace_back(x_index, y_index);
}
