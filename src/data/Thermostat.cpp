#include "Thermostat.h"
#include "utils/ArrayUtils.h"

Thermostat::Thermostat(double targetTemp, double maxDeltaTemp, size_t thermostatInterval, size_t dimensions)
    : targetTemp(targetTemp),
      maxDeltaTemp(maxDeltaTemp),
      thermostatInterval(thermostatInterval),
      dimensions(dimensions) {}

//E_kin = sum_particles (m* <v,v>/2)
//E_kin = #dims*#particles*T/2
double Thermostat::getCurrentTemperature(const std::unique_ptr<ParticleContainer>& pc) const {
    double doubledKinEnergy = 0;
    for (auto it = pc->begin(); it != pc->end(); ++it) {
        std::array<double, 3> v = (*it)->getV();
        doubledKinEnergy += (*it)->getM() * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }
    return doubledKinEnergy / (dimensions * pc->getParticles().size());
}

void Thermostat::scaleWithBeta(std::unique_ptr<ParticleContainer>& pc) {
    const double currTemp = getCurrentTemperature(pc);
    const double tempChange = std::min(std::abs(targetTemp - currTemp), maxDeltaTemp);
    const double new_temperature = currTemp + tempChange * (targetTemp > currTemp ? 1 : -1);

    const double beta = std::sqrt(new_temperature / currTemp);

    for (auto it = pc->begin(); it != pc->end(); ++it) {
        (*it)->setV(beta * (*it)->getV());
    }
}

void Thermostat::InitSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc) {
    if (dimensions < 2 || dimensions > 3) {
        spdlog::error("Invalid number of dimensions: {}. Must be 2 or 3.", dimensions);
        exit(-1);
    }

    for (auto it = pc->begin(); it != pc->end(); ++it) {
        (*it)->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (*it)->getM()), dimensions));
    }
}

double Thermostat::getTargetTemp() const { return targetTemp; }

double Thermostat::getMaxDeltaTemp() const { return maxDeltaTemp; }

size_t Thermostat::getThermostatInterval() const { return thermostatInterval; }

size_t Thermostat::getDimensions() const { return dimensions; }