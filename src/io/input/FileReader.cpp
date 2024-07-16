/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"


FileReader::FileReader() = default;

FileReader::~FileReader() = default;

void FileReader::readFile(ParticleContainer &particles, const char *filename, std::array<double,3> domainStart){
  std::array<double, 3> x;
  std::array<double, 3> v;
  std::array<double, 3> f;
  std::array<double, 3> oldF;
  double m;
  double epsilon;
  double sigma;
  int type;
  bool is_outer;
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
      datastream >> epsilon;
      if (datastream.fail()) {
        epsilon = 5;
        sigma = 1;
        Particle *particle = new Particle(x,v,m,false,0,epsilon,sigma,domainStart);
        particles.addParticle(particle);
        continue;
      }
      datastream >> sigma;
      if (datastream.fail()) {
        spdlog::error("Error reading file: failed to parse line {}", i);
        exit(-1);
      }
      datastream >> type;
      if (datastream.fail()) {
        spdlog::error("Error reading file: failed to parse line {}", i);
        exit(-1);
      }
      datastream >> is_outer;
      if (datastream.fail()) {
        spdlog::error("Error reading file: failed to parse line {}", i);
        exit(-1);
      }
      
      Particle *particle = new Particle(x,v,m,is_outer,type,epsilon,sigma,domainStart);
      particles.addParticle(particle);

      for (auto &fj : f) {
        datastream >> fj;
      }
      for (auto &oldFj : oldF) {
        datastream >> oldFj;
      }
      if (!datastream.eof()) {
        particle->setF(f);
        particle->setOldF(oldF);
      }
    }
  } else {
    spdlog::error("Error: could not open file {}", filename);
    exit(-1);
  }
}