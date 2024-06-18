#include "Thermostat.h"

Thermostat::Thermostat() 
    : initialTemp(0), targetTemp(0), maxDeltaTemp(0), nThermostat(0), dimensions(2) {
}

Thermostat::Thermostat(double initialTemp, double targetTemp, double maxDeltaTemp, size_t nThermostat, size_t dimensions)
    : targetTemp(targetTemp),
      maxDeltaTemp(maxDeltaTemp == 0 ? std::numeric_limits<double>::infinity() : maxDeltaTemp),
      nThermostat(nThermostat),
      dimensions(dimensions) {}

//E_kin = sum_particles (m* <v,v>/2)
//E_kin = #dims*#particles*T/2
double Thermostat::getCurrentTemperature(const std::unique_ptr<ParticleContainer>& pc) const {
    double doubledKinEnergy = 0;
     size_t particleCount = pc->getParticles().size();
    if (particleCount == 0) {
        spdlog::warn("Particle container is empty. Returning temperature as 0.");
        return 0.0;
    }
    for (auto it = pc->begin(); it != pc->end(); ++it) {
        std::array<double, 3> v = (*it)->getV();
        doubledKinEnergy += (*it)->getM() * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }
    return doubledKinEnergy / (dimensions * particleCount);
}

void Thermostat::scaleWithBeta(std::unique_ptr<ParticleContainer>& pc) {
    const double currTemp = getCurrentTemperature(pc);
    if (currTemp == 0.0) {
        spdlog::warn("Current temperature is 0. Skipping scaling.");
        return;
    }

     double new_temperature;
    if (std::isinf(maxDeltaTemp)) {
        new_temperature = targetTemp;
    } else {
        const double tempChange = std::min(std::abs(targetTemp - currTemp), maxDeltaTemp);
        new_temperature = currTemp + tempChange * (targetTemp > currTemp ? 1 : -1);
    }

    const double beta = std::sqrt(new_temperature / currTemp);

    for (auto it = pc->begin(); it != pc->end(); ++it) {
        (*it)->setV(beta * (*it)->getV());
    }
}

void Thermostat::initSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc) {
    if (dimensions < 2 || dimensions > 3) {
        spdlog::error("Invalid number of dimensions: {}. Must be 2 or 3.", dimensions);
        exit(-1);
    }

    for (auto it = pc->begin(); it != pc->end(); ++it) {
        (*it)->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (*it)->getM()), dimensions));
    }
}

double Thermostat::getInitialTemp() const { return initialTemp; }

double Thermostat::getTargetTemp() const { return targetTemp; }

double Thermostat::getMaxDeltaTemp() const { return maxDeltaTemp; }

size_t Thermostat::getNThermostat() const { return nThermostat; }

size_t Thermostat::getDimensions() const { return dimensions; }