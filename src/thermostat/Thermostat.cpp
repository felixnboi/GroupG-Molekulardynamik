#include "Thermostat.h"

Thermostat::Thermostat() = default;

Thermostat::Thermostat(ThermostatData thermostat_data) : thermostat_data(thermostat_data) {}

double Thermostat::getCurrentTemperature(const std::unique_ptr<ParticleContainer>& pc) const {
    double doubled_kin_energy = 0;
    const size_t particle_count = pc->getParticles().size();
    const size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();

    if (particle_count == 0) {
        spdlog::warn("Particle container is empty. Returning temperature as 0.");
        return 0.0;
    }

    for (const auto& particle : pc->getParticles()) {
        double tmp = 0;
        auto v = particle->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            tmp += (v[d] * v[d]);
        }
        doubled_kin_energy += tmp * particle->getM();
    }
    return doubled_kin_energy / (brownian_motion_dimension * particle_count);
}

void Thermostat::scaleWithBeta(std::unique_ptr<ParticleContainer>& pc) {
    const double curr_temp = getCurrentTemperature(pc);
    const double max_delta_temp = thermostat_data.getMaxDeltaTemp();
    const double target_temp = thermostat_data.getTargetTemp();
    const size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();
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

    for (auto& particle : pc->getParticles()) {
         auto v = particle->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            v[d] *= beta;
        }
        particle->setV(v);
    }
}

void Thermostat::scaleWithBetaFluid(std::unique_ptr<ParticleContainer>& pc) {
    double doubled_kin_energy = 0;
    size_t particle_count = 0;
    const size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();
    std::array<double, 3> average_velocity = {0., 0., 0.};

    // Calculate average velocity
    for (const auto& particle : pc->getParticles()) {
        if (particle->getIsOuter()) continue;
        auto v = particle->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            average_velocity[d] += v[d];
        }
        ++particle_count;
    }

    for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            average_velocity[d] /= (particle_count);
        }

    // Calculate kinetic energy without average velocity
    for (const auto& particle : pc->getParticles()) {
        if (particle->getIsOuter()) continue;
        double tmp = 0;
        auto v = particle->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            double corrected_v = v[d] - average_velocity[d];
            tmp += (corrected_v * corrected_v);
        }
        doubled_kin_energy += tmp * particle->getM();
    }
    double curr_temp_without_average = doubled_kin_energy / (brownian_motion_dimension * particle_count);

    if (curr_temp_without_average == 0.0) {
        spdlog::warn("Current temperature is 0. Skipping scaling.");
        return;
    }

    const double max_delta_temp = thermostat_data.getMaxDeltaTemp();
    const double target_temp = thermostat_data.getTargetTemp();

    double new_temperature;
    if (std::isinf(max_delta_temp)) {
        new_temperature = target_temp;
    } else {
        const double tempChange = std::min(std::abs(target_temp - curr_temp_without_average), max_delta_temp);
        new_temperature = curr_temp_without_average + tempChange * (target_temp > curr_temp_without_average ? 1 : -1);
    }

    const double beta = std::sqrt(new_temperature / curr_temp_without_average);
    
    // Scale velocities
    for (auto& particle : pc->getParticles()) {
        if (particle->getIsOuter()) continue;
        auto v = particle->getV();
        for (size_t d = 0; d < brownian_motion_dimension; ++d) {
            double corrected_v = (v[d] - average_velocity[d]) * beta;
            v[d] = corrected_v + average_velocity[d];
        }
        particle->setV(v);
    }
}

void Thermostat::initSystemTemperature(double new_initialTemp, std::unique_ptr<ParticleContainer>& pc) {
    size_t brownian_motion_dimension = thermostat_data.getBrownianMotionDimension();

    if (brownian_motion_dimension < 2 || brownian_motion_dimension > 3) {
        spdlog::error("Invalid number of brownian_motion_dimension: {}. Must be 2 or 3.", brownian_motion_dimension);
        exit(-1);
    }

    for(auto& particle : pc->getParticles()){
        particle->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_initialTemp / particle->getM()), brownian_motion_dimension));
    }
}

const ThermostatData& Thermostat::getThermostatData() const {
    return thermostat_data;
}

void Thermostat::setThermostatData(const ThermostatData& new_thermostat_data){
    thermostat_data = new_thermostat_data;
}

