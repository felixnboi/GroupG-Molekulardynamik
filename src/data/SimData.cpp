#include "SimData.h"

SimData::SimData() = default;

SimData::SimData(std::string input_file, std::string baseName, unsigned write_frequency, double start_time, double end_time, double delta_t, 
    std::string force_str, std::string algorithm, std::string loglevel, std::array<std::string, 6> boundary, double cutoff_radius, size_t dimensions,
    std::array<double, 3> domain, double sigma, double epsilon, double initialTemp)
    : input_file(std::move(input_file)), 
      baseName(std::move(baseName)), 
      write_frequency(write_frequency), 
      start_time(start_time), 
      end_time(end_time), 
      delta_t(delta_t), 
      force_str(std::move(force_str)), 
      algorithm(std::move(algorithm)), 
      loglevel(std::move(loglevel)), 
      boundary(std::move(boundary)), 
      cutoff_radius(cutoff_radius), 
      dimensions(dimensions),
      domain(std::move(domain)), 
      sigma(sigma), 
      epsilon(epsilon),
      initialTemp(initialTemp) {}

SimData::~SimData() = default;

std::string SimData::getInputFile() const { return input_file; }
std::string SimData::getBaseName() const { return baseName; }
unsigned SimData::getWriteFrequency() const { return write_frequency; }
double SimData::getStartTime() const { return start_time; }
double SimData::getEndTime() const { return end_time; }
double SimData::getDeltaT() const { return delta_t; }
std::string SimData::getForceStr() const { return force_str; }
std::string SimData::getAlgorithm() const { return algorithm; }
std::string SimData::getLoglevel() const { return loglevel; }
std::array<std::string, 6> SimData::getBoundary() const { return boundary; }
double SimData::getCutoffRadius() const { return cutoff_radius; }
double SimData::getDimensions() const { return dimensions; }
std::array<double, 3> SimData::getDomain() const { return domain; }
double SimData::getSigma() const { return sigma; }
double SimData::getEpsilon() const { return epsilon; }
double SimData::getInitialTemp() const { return initialTemp; }


void SimData::setInputFile(const std::string& new_input_file) { input_file = new_input_file; }
void SimData::setBaseName(const std::string& new_base_name) { baseName = new_base_name; }
void SimData::setWriteFrequency(unsigned new_write_frequency) { write_frequency = new_write_frequency; }
void SimData::setStartTime(double new_start_time) { start_time = new_start_time; }
void SimData::setEndTime(double new_end_time) { end_time = new_end_time; }
void SimData::setDeltaT(double new_delta_t) { delta_t = new_delta_t; }
void SimData::setForceStr(const std::string& new_force_str) { force_str = new_force_str; }
void SimData::setAlgorithm(const std::string& new_algorithm) { algorithm = new_algorithm; }
void SimData::setLoglevel(const std::string& new_loglevel) { loglevel = new_loglevel; }
void SimData::setBoundary(const std::array<std::string, 6>& new_boundary) { boundary = new_boundary; }
void SimData::setCutoffRadius(double new_cutoff_radius) { cutoff_radius = new_cutoff_radius; }
void SimData::setDimensions(double new_dimensions_number) { dimensions = new_dimensions_number; }
void SimData::setDomain(const std::array<double, 3>& new_domain) { domain = new_domain; }
void SimData::setSigma(double new_sigma) { sigma = new_sigma; }
void SimData::setEpsilon(double new_epsilon) { epsilon = new_epsilon; }
void SimData::setInitialTemp(double new_initialTemp) { initialTemp = new_initialTemp; }

