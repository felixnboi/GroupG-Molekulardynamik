#include "OpenMPData.h"

OpenMPData::OpenMPData() = default;

OpenMPData::OpenMPData(bool openmp_flag, size_t num_threads, std::string strategy)
    : openmp_flag(openmp_flag),
      num_threads(num_threads),
      strategy(std::move(strategy)) {}

bool OpenMPData::getOpenMPFlag() const { return openmp_flag; }
size_t OpenMPData::getNumThreads() const { return num_threads; }
std::string OpenMPData::getStrategy() const { return strategy; }

void OpenMPData::setOpenMPFlag(bool new_openmp_flag) { openmp_flag = new_openmp_flag; }
void OpenMPData::setNumThreads(size_t new_num_threads) { num_threads = new_num_threads; }
void OpenMPData::setStrategy(const std::string& new_strategy) { strategy = new_strategy; }
