#include "ParticleGenerator.h"

ParticleContainer generateCuboid(double x, double y, double z, size_t sizeX, size_t sizeY, size_t sizeZ, double distance, double mass, double velocityX, double velocityY, double velocityZ) {
    ParticleContainer particles;
    double yClone = y;
    double zClone = z;
    double averageBrwoinanMotion = 0.1;
    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t j = 0; j < sizeY; j++)
        {
            for (size_t k = 0; k < sizeZ; k++) {
                std::array<double, 3> brownianMotion = maxwellBoltzmannDistributedVelocity(averageBrwoinanMotion, 3);
                particles.addParticle(*(new Particle({x,y,z},{velocityX * brownianMotion[0],velocityY * brownianMotion[1],velocityZ * brownianMotion[2]},mass,0)));
                z += distance;
            }
            z = zClone;
            y += distance;
        }
        y = yClone;
        x += distance;
    }
    
}