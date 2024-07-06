#include "ThermostatData.h"

ThermostatData::ThermostatData() 
    : thermostat_flag(false), 
      n_thermostat(1000), 
      brownian_motion_dimension(2), 
      target_temp_flag(false), 
      target_temp(0), 
      max_delta_temp(0),
      init_temp_flag(false),  
      init_temp(0) {}

ThermostatData::ThermostatData(bool thermostat_flag, size_t n_thermostat, size_t brownian_motion_dimension, bool target_temp_flag, 
double target_temp, double max_delta_temp, bool init_temp_flag, double init_temp)
    : thermostat_flag(thermostat_flag),
      n_thermostat(n_thermostat), 
      brownian_motion_dimension(brownian_motion_dimension),
      target_temp_flag(target_temp_flag),  
      target_temp(target_temp), 
      max_delta_temp(max_delta_temp), 
      init_temp_flag(init_temp_flag),
      init_temp(init_temp) {}

bool ThermostatData::getThermostatFlag() const { return thermostat_flag; }
bool ThermostatData::getTargetTempFlag() const { return target_temp_flag; }
double ThermostatData::getTargetTemp() const { return target_temp; }
double ThermostatData::getMaxDeltaTemp() const { return max_delta_temp; }
size_t ThermostatData::getNThermostat() const { return n_thermostat; }
size_t ThermostatData::getBrownianMotionDimension() const { return brownian_motion_dimension; }
bool ThermostatData::getInitTempFlag() const { return init_temp_flag; }
double ThermostatData::getInitTemp() const { return init_temp; }

void ThermostatData::setThermostatFlag(bool new_thermostat_flag) { thermostat_flag = new_thermostat_flag; }
void ThermostatData::setTargertTempFlag(bool new_target_temp_flag) { target_temp_flag = new_target_temp_flag; }
void ThermostatData::setTargetTemp(double new_target_temp) { target_temp = new_target_temp; }
void ThermostatData::setMaxDeltaTemp(double new_max_delta_temp) { max_delta_temp = new_max_delta_temp; }
void ThermostatData::setNThermostat(size_t new_n_thermostat) { n_thermostat = new_n_thermostat; }
void ThermostatData::setBrownianMotionDimension(size_t new_brownian_motion_dimension) { brownian_motion_dimension = new_brownian_motion_dimension; }
void ThermostatData::setInitTempFlag(bool new_init_temp_flag) { init_temp_flag = new_init_temp_flag; }
void ThermostatData::setInitTemp(double new_init_temp) { init_temp = new_init_temp; }