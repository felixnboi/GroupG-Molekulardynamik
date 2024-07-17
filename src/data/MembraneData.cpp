#include "MembraneData.h"
#include <stddef.h>

MembraneData::MembraneData() = default;

MembraneData::MembraneData(bool error_flag, bool membrane_flag, double r0, double k, double f_z_up, size_t sizeX, size_t sizeY)
  : error_flag(error_flag), 
    membrane_flag(membrane_flag), 
    r0(r0), 
    k(k), 
    f_z_up(f_z_up), 
    sizeX(sizeX), 
    sizeY(sizeY) {}

bool MembraneData::getErrorFlag() const { return error_flag; }
bool MembraneData::getMembraneFlag() const { return membrane_flag; }
double MembraneData::getR0() const { return r0; }
double MembraneData::getK() const { return k; }
double MembraneData::getF_z_up() const { return f_z_up; }
size_t MembraneData::getSizeX() const { return sizeX; }
size_t MembraneData::getSizeY() const { return sizeY; }
const std::vector<std::pair<size_t, size_t>>& MembraneData::getParticleUp() const { return particle_up; }

void MembraneData::setErrorFlag(bool new_error_flag) { error_flag = new_error_flag; }
void MembraneData::setMembraneFlag(bool new_membrane_flag) { membrane_flag = new_membrane_flag; }
void MembraneData::setR0(double new_r0) { r0 = new_r0; }
void MembraneData::setK(double new_k) { k = new_k; }
void MembraneData::setF_z_up(double new_f_z_up) { f_z_up = new_f_z_up; }
void MembraneData::setSizeX(size_t new_sizeX) {sizeX = new_sizeX; }
void MembraneData::setSizeY(size_t new_sizeY) {sizeY = new_sizeY; }
void MembraneData::setParticleUp(const std::vector<std::pair<size_t, size_t>>& new_particle_up) { particle_up = new_particle_up; }

void MembraneData::addParticleUp(size_t x_index, size_t y_index) {
  particle_up.emplace_back(x_index, y_index);
}
