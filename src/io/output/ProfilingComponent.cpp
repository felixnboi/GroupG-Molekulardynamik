#include "ProfilingComponent.h"

bool ProfilingComponent::ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void ProfilingComponent::profile(size_t bins, std::array<double,3> domainSize, ParticleContainer& pc, const std::string &fileName) {
    std::string tmp{fileName};
    if (!ends_with(tmp, ".csv")) tmp.append(".csv");

    std::vector<int> densities(bins, 0);
    std::vector<double> velocities(3 * bins, 0.0);
    const double bin_size = domainSize[0] / bins;
    const size_t size = pc.getParticleCount();
    const std::vector<Particle*>& particles = pc.getParticles();

    for (size_t j = 0; j < size; ++j) {
        auto& particle = particles[j];

        const double particle_x = particle->getX()[0];
        size_t bin_index = static_cast<size_t>(particle_x / bin_size);
        if (bin_index >= bins) continue;

        densities[bin_index]++;
        velocities[3 * bin_index + 0] += particle->getV()[0];
        velocities[3 * bin_index + 1] += particle->getV()[1];
        velocities[3 * bin_index + 2] += particle->getV()[2];

    }

    // computing averages
    std::vector<double> velocity_avg_x(bins, 0.0);
    std::vector<double> velocity_avg_y(bins, 0.0);
    std::vector<double> velocity_avg_z(bins, 0.0);
    for (size_t i = 0; i < bins; ++i) {
        if (densities[i] != 0) {
            velocity_avg_x[i] = velocities[3 * i + 0] / densities[i];
            velocity_avg_y[i] = velocities[3 * i + 1] / densities[i];
            velocity_avg_z[i] = velocities[3 * i + 2] / densities[i];
        }
    }

    // writing results to a CSV file
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << "Bin, Density, Velocity_X, Velocity_Y, Velocity_Z\n";
        for (size_t i = 0; i < bins; ++i) {
            file << i << ", " << densities[i] << ", " << velocity_avg_x[i] << ", " 
            << velocity_avg_y[i] << ", " << velocity_avg_z[i] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << fileName << "\n";
    }
}