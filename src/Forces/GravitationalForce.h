#include "utils/ArrayUtils.h"
#include <math.h>
#include "ParticleContainer.h"
#include "Force.h"

class GravitationalForce : public Force
{
public:
    GravitationalForce();
    ~GravitationalForce();
    void calculateF(ParticleContainer &particles) override;
};