/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "spdlog/spdlog.h"

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

    while (tmp_string.empty() or tmp_string[0] == '#') {
      spdlog::warn("Empty line or comment found: {}", tmp_string);
      getline(input_file, tmp_string);
      spdlog::info("Read line: {}", tmp_string);
    }

    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    if(num_particles == 0)
    {
      spdlog::error("Error: Particle file contains no praticles\n");
      exit(-1);
    }
    spdlog::info("Reading {} particles.", num_particles);
    getline(input_file, tmp_string);
    spdlog::info("Read line: {}", tmp_string);

    for (int i = 0; i < num_particles; i++) {
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
      Particle tmp{x,v,m};
      particles.addParticle(tmp);

      getline(input_file, tmp_string);
      spdlog::info("Read line: {}", tmp_string);
    }
  } else {
    spdlog::error("Error: could not open file {}", filename);
    exit(-1);
  }
}

