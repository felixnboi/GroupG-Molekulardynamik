#include "utils/ArrayUtils.h"
#include <math.h>
#include "ParticleContainer.h"
#include "Force.h"

class Lenard_Jones_Force : public Force
{
public:
    Lenard_Jones_Force();
    ~Lenard_Jones_Force();
    void calculateF(ParticleContainer &particles) override;
};
