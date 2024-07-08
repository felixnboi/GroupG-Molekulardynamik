#include <fstream>
#include "ProfilingComponent.h"


ProfilingComponent::ProfilingComponent() {}

ProfilingComponent::~ProfilingComponent() {}


    bool ProfilingComponent::ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    void ProfilingComponent::profile(size_t N_bins, std::array<double, 3> domainSize, ParticleContainer &pc, const std::string &fileName) {
        std::string tmp{fileName};
        if (!ends_with(tmp, ".csv")) tmp.append(".csv");

        std::vector<int> densities(N_bins, 0);
        std::vector<double> velocities(3 * N_bins, 0.0);
        int count = 0;
        const double bin_size = domainSize[0] / N_bins;
        size_t size = pc.getParticleCount();
        const std::vector<std::shared_ptr<Particle>>& particles = pc.getParticles();

        for (size_t j = 0; j < size; ++j) {
        auto& particle = particles[j];
        double x0 = 0; // since our domain starts at (0,0,0)
        for (int i = 0; i < N_bins; ++i, x0 += bin_size) {
            if (particle->getX()[0] < x0 || particle->getX()[0] >= x0 + bin_size) continue;
            densities[i]++;
            velocities[3 * i + 0] += particle->getV()[0];
            velocities[3 * i + 1] += particle->getV()[1];
            velocities[3 * i + 2] += particle->getV()[2];
        }
        }
    
    // computing averages
    std::vector<double> velocity_avg_x(N_bins, 0.0);
    std::vector<double> velocity_avg_y(N_bins, 0.0);
    std::vector<double> velocity_avg_z(N_bins, 0.0);
    for (int i = 0; i < N_bins; ++i) {
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
        for (int i = 0; i < N_bins; ++i) {
            file << i << ", " << densities[i] << ", " << velocity_avg_x[i] << ", " 
            << velocity_avg_y[i] << ", " << velocity_avg_z[i] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << fileName << "\n";
    }
    }