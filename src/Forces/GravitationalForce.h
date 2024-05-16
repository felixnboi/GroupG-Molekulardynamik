#include "utils/ArrayUtils.h"
#include <math.h>
#include "ParticleContainer.h"
#include "Force.h"

/**
 * @brief This class implements the Force interface to calculate gravitational forces between particles
 * based on their masses and positions.
 */
class GravitationalForce : public Force
{
public:
    /**
     * @brief Default constructor.
     */
    GravitationalForce();
    /**
     * @brief Destructor.
     */
    ~GravitationalForce();
    /**
     * @brief Calculates gravitational forces between particles.
     * 
     * This method calculates gravitational forces between particles based on their masses and positions.
     * It iterates over all pairs of particles and updates their force vectors accordingly.
     * 
     * @param particles The ParticleContainer containing the particles for which gravitational forces are to be calculated.
     */
    void calculateF(ParticleContainer &particles) override;
};