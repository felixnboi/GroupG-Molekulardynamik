
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <math.h>

/**** forward declaration of the calculation functions ****/

/**
 * calculate the euclidean norm of the distance between two particles
 */
double euclidean_norm_x(Particle particle1, Particle particle2);

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "./molsym filename" << std::endl;
  }

  FileReader fileReader;
  fileReader.readFile(particles, argsv[1]);

  double current_time = start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {
    // calculate new x
    calculateX();
    // calculate new f
    calculateF();
    // calculate new v
    calculateV();

    iteration++;
    if (iteration % 10 == 0) {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void calculateF() {
  std::list<Particle>::iterator iterator_i;
  iterator_i = particles.begin();
  std::list<Particle>::iterator iterator_j;

  for(auto &p : particles){
    auto old_F = p.getOldF();
    auto cur_F = p.getF();
    for(int i = 0; i<3 ;i++){
      old_F.at(i) = cur_F.at(i);
      cur_F.at(i) = 0;
    }
  }
  for (int i = 0; i<particles.size()-1; i++) {
    auto cur_particle_i = *iterator_i;
    auto m_i = cur_particle_i.getM();
    auto cur_x_i = cur_particle_i.getX();
    auto cur_F_i = cur_particle_i.getF();
    for (iterator_j = iterator_i++; iterator_j!=particles.end(); iterator_j++) {
      auto cur_particle_j = *iterator_j;
      auto m_j = cur_particle_j.getM();
      auto norm = euclidean_norm_x(cur_particle_i, cur_particle_j);
      auto cur_x_j = cur_particle_j.getX(); 
      auto cur_F_j = cur_particle_j.getF();
      for(int k = 0; k<3; k++){
        double tmp = m_i * m_j / pow(norm, 3) * (cur_x_j.at(k) - cur_x_i.at(k));
        cur_F_i.at(k) += tmp;
        cur_F_j.at(k) -= tmp;
      }
    }
  }
}

double euclidean_norm_x(Particle particle1, Particle particle2){
  double sum = 0;
  for (int i = 0; i<3; i++){
    sum += pow(particle1.getX().at(i) - particle2.getX().at(i), 2);
  }
  return pow(sum, 0.5);
}

void calculateX() {
  for (auto &p : particles) {
    auto m = p.getM();
    auto cur_x = p.getX();
    auto cur_v = p.getV();
    auto cur_F = p.getF();
    for(int i = 0; i<3; i++){
      cur_x.at(i) = cur_x.at(i) + delta_t * cur_v.at(i) + delta_t * delta_t * cur_F.at(i) / (2*m); 
    }
  }
}

void calculateV() {
  for (auto &p : particles) {
    auto m = p.getM();
    auto cur_x = p.getX();
    auto cur_v = p.getV();
    auto cur_F = p.getF();
    auto old_F = p.getOldF();
    for(int i = 0; i<3; i++){
      cur_v.at(i) = cur_v.at(i) + delta_t * (old_F.at(i) + cur_F.at(i)) / (2*m);
    }
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);
}
