#include "Thermostat.h"

Thermostat::Thermostat() 
    : targetTemp(0), maxDeltaTemp(0), nThermostat(1000), dimensions(2) {
}

Thermostat::Thermostat(double targetTemp, double maxDeltaTemp, size_t nThermostat, size_t dimensions)
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
        auto v = (*it)->getV();
        doubledKinEnergy = 0;
        for (size_t d = 0; d < dimensions; ++d) {
            doubledKinEnergy += (v[d] * v[d]);
        }
        doubledKinEnergy = doubledKinEnergy * (*it)->getM();
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
         auto v = (*it)->getV();
        for (size_t d = 0; d < dimensions; ++d) {
            v[d] *= beta;
        }
        (*it)->setV(v);
    }
}

void Thermostat::initSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc) {
    if (dimensions < 2 || dimensions > 3) {
        spdlog::error("Invalid number of dimensions: {}. Must be 2 or 3.", dimensions);
        exit(-1);
    }

    for(auto& it : *pc){
    //for (auto it = pc->begin(); it != pc->end(); ++it) {
        //(*it)->setV(std::sqrt(new_initialTemp / (*it)->getM()) * maxwellBoltzmannDistributedVelocity(0.1, dimensions));
        (it)->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (it)->getM()), dimensions));
        //(*it)->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (*it)->getM()), dimensions));
    }
}

double Thermostat::getTargetTemp() const { return targetTemp; }

double Thermostat::getMaxDeltaTemp() const { return maxDeltaTemp; }

size_t Thermostat::getNThermostat() const { return nThermostat; }

size_t Thermostat::getDimensions() const { return dimensions; }

void Thermostat::setTargetTemp(double new_targetTemp) { targetTemp = new_targetTemp; }

void Thermostat::setMaxDeltaTemp(double new_maxDeltaTemp) { maxDeltaTemp = new_maxDeltaTemp; }

void Thermostat::setNThermostat(size_t new_NThermostat) {nThermostat = new_NThermostat; }

void Thermostat::setDimensions(size_t new_dimensions) {dimensions = new_dimensions;}