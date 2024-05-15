#pragma once
#include "ParticleContainer.h"

class Force
{
public:
    virtual void calculateF(ParticleContainer &particles){};
    virtual ~Force() {}
};