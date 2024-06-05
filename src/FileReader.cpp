/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"


FileReader::FileReader() = default;

FileReader::~FileReader() = default;

void FileReader::readFile(ParticleContainer &particles, const char *filename){
  std::array<double, 3> x;
  std::array<double, 3> v;
  double m;
  int num_particles = 0;

  std::ifstream input_file(filename);
  std::string tmp_string;

  if (input_file.is_open()) {

    getline(input_file, tmp_string);
    spdlog::info("Read line: {}", tmp_string);

    while (tmp_string.empty() || tmp_string[0] == '#') {
      spdlog::warn("Empty line or comment found: {}", tmp_string);
      getline(input_file, tmp_string);
      spdlog::info("Read line: {}", tmp_string);
    }

    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    if(num_particles <= 0){
      spdlog::error("Error: Particle file contains no praticles\n");
      exit(-1);
    }
    spdlog::info("Reading {} particles.", num_particles);

    particles.reserve(num_particles);

    for (int i = 0; i < num_particles; i++) {
      if (!getline(input_file, tmp_string)) {
            spdlog::error("Error reading file: unexpected end of file at line {}", i);
            exit(-1);
        }
        spdlog::info("Read line: {}", tmp_string);

      std::istringstream datastream(tmp_string);
      for (auto &xj : x) {
        datastream >> xj;
      }
      for (auto &vj : v) {
        datastream >> vj;
      }
      if (datastream.eof()) {
        spdlog::error("Error reading file: eof reached unexpectedly reading from line {}", i);
        exit(-1);
      }
      datastream >> m;
      if (datastream.fail()) {
                spdlog::error("Error reading file: failed to parse line {}", i);
                exit(-1);
            }
            
      particles.addParticle(std::make_shared<Particle> (x,v,m));

      spdlog::info("Particle generated with position ({}, {}, {}) and velocity ({}, {}, {}) and mass {}", x[0], x[1], x[2], v[0], v[1], v[2], m);
      //getline(input_file, tmp_string);
      //spdlog::info("Read line: {}", tmp_string);
    }
  } else {
    spdlog::error("Error: could not open file {}", filename);
    exit(-1);
  }
}