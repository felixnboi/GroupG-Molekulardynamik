#include "Thermostat.h"

Thermostat::Thermostat() = default;

Thermostat::Thermostat(ThermostatData thermostat_data) : thermostat_data(thermostat_data) {}

double Thermostat::getCurrentTemperature(const std::unique_ptr<ParticleContainer>& pc) const {
    double doubled_kin_energy = 0;
    size_t particle_count = pc->getParticles().size();
    size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();

    if (particle_count == 0) {
        spdlog::warn("Particle container is empty. Returning temperature as 0.");
        return 0.0;
    }

    for (auto it = pc->begin(); it != pc->end(); ++it) {
        double tmp = 0;
        auto v = (*it)->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            tmp += (v[d] * v[d]);
        }
        doubled_kin_energy += tmp * (*it)->getM();
    }
    return doubled_kin_energy / (brownian_motion_dimension * particle_count);
}

void Thermostat::scaleWithBeta(std::unique_ptr<ParticleContainer>& pc) {
    const double curr_temp = getCurrentTemperature(pc);
    double max_delta_temp = thermostat_data.getMaxDeltaTemp();
    double target_temp = thermostat_data.getTargetTemp();
    size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();
    if (curr_temp == 0.0) {
        spdlog::warn("Current temperature is 0. Skipping scaling.");
        return;
    }

    double new_temperature;
    if (std::isinf(max_delta_temp)) {
        new_temperature = target_temp;
    } else {
        const double tempChange = std::min(std::abs(target_temp - curr_temp), max_delta_temp);
        new_temperature = curr_temp + tempChange * (target_temp > curr_temp ? 1 : -1);
    }

    const double beta = std::sqrt(new_temperature / curr_temp);

    for (auto it = pc->begin(); it != pc->end(); ++it) {
         auto v = (*it)->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            v[d] *= beta;
        }
        (*it)->setV(v);
    }
}

void Thermostat::initSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc) {
    size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();

    if (brownian_motion_dimension < 2 || brownian_motion_dimension > 3) {
        spdlog::error("Invalid number of brownian_motion_dimension: {}. Must be 2 or 3.", brownian_motion_dimension);
        exit(-1);
    }

    for(auto& it : *pc){
    //for (auto it = pc->begin(); it != pc->end(); ++it) {
        //(*it)->setV(std::sqrt(new_initialTemp / (*it)->getM()) * maxwellBoltzmannDistributedVelocity(0.1, brownian_motion_dimension));
        auto tmp = maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (it)->getM()), brownian_motion_dimension);
        //(it)->setV(maxwellBoltzmannDistributedVelocity(0.1, brownian_motion_dimension));
        (it)->setV(tmp);
        std::cout <<"velocityx: " << tmp[0] << " y: " << tmp[1] << " z: " << tmp[2] << std::endl;

        //(*it)->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / (*it)->getM()), brownian_motion_dimension));
    }
}

const ThermostatData& Thermostat::getThermostatData() const {
    return thermostat_data;
}

void Thermostat::setThermostatData(const ThermostatData& new_thermostat_data){
    thermostat_data = new_thermostat_data;
}

