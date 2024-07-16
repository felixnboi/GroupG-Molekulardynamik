#pragma once
#include "ParticleContainer.h"
 
/**
 * @brief This class implements the ParticleContainer interface to store particles in different
 * linked cells and be able to control them moving in between cells.
 */
class ParticleContainerLinkedCell : public ParticleContainer{
public:

    /**
     * @brief Constructor for ParticleContainerLinkedCell.
     * 
     * @param sizeX The size of the container in the X dimension.
     * @param sizeY The size of the container in the Y dimension.
     * @param sizeZ The size of the container in the Z dimension.
     * @param radius Cut-off radius.
     */
    ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius);

    /**
     * @brief Destructor for ParticleContainerLinkedCell.
     */
    ~ParticleContainerLinkedCell();

    /**
     * @brief Returns the number of particles in this particle container.
     * 
     * @return The number of particles in this container.
     */
    size_t getParticleCount() const override;

    /**
     * @brief Reserves space for a given number of particles.
     * 
     * @param size The number of particles to reserve space for.
     */
    void reserve(size_t size) override;

    /**
     * @brief Adds a particle to the container.
     * 
     * @param particle The particle to add.
     */
    void addParticle(Particle* particle) override;

    /**
     * @brief Returns an iterator to the beginning of the particles.
     * 
     * @return An iterator to the beginning of the particles.
     */
    ParticleIterator begin() override;

    /**
     * @brief Returns an iterator to the end of the particles.
     * 
     * @return An iterator to the end of the particles.
     */
    ParticleIterator end() override;

    /**
    * @brief Updates the locations of particles considering outflow and periodic boundary conditions.
    * 
    * @param outflowflag Array indicating which faces of the simulation domain have outflow boundaries.
    * @param peridicflag Array indicating which dimensions have periodic boundaries.
    */
    void updateLoctions(std::array<bool,6> outflowflag, std::array<bool,3> peridicflag);

    /**
     * @brief Gets the particles in the container.
     * 
     * @return A reference to the vector of particles.
     */
    const std::vector<Particle*>& getParticles() const override;

    /**
     * @brief Gets the size of the container.
     * 
     * @return The size of the container as an array of three doubles.
     */
    const std::array<double, 3> getSize() const;

    /**
     * @brief Getter for the size of each cell.
     * 
     * @return The size of each cell as an array of three doubles.
     */
    const std::array<double, 3> getCellSize() const;    
    
    /**
    * @brief Getter for the number of cells in each dimension.
    * 
    * @return The number of cells in each dimension as an array.
    */
    const std::array<size_t, 3> getCellCount() const;

    /**
     * @brief Getter for the squared interaction radius.
     * 
    * @return The squared interaction radius.
    */
    const double getRadiusSquared() const;

    /**
     * @brief Getter for the pairs of particles within the interaction radius. (Ingnores periodic boundery)
     * 
     * @return A vector of these pairs of particles.
     */
    std::vector<std::pair<Particle*, Particle*>> getParticlePairs() override;

    /**
    * @brief Gets the pairs of particles that could interact through EVERY periodic boundery where the flag is set and only those. (Doesn't always check if they are within the cut off radius).
    * 
    * @param pFlag Array indicating for which dimensions particles should interact through the periodic boundaries.
    * 
    * @return A vector of these pairs of particles.
    */
    std::vector<std::pair<Particle*, Particle*>> getParticlePairsPeriodic(std::array<bool, 3> pFlag);

    /**
     * @brief Gets the particles located at the boundary of the container.
     * 
     * @return A vector of shared pointers to the particles at the boundary.
     */
    std::vector<Particle*> getBoundary() const;

    /**
     * @brief Gets the particles located in the halo.
     * 
     * @return A vector of shared pointers to the particles in the halo.
     */
    std::vector<Particle*> getHalo() const;

    /**
     * @brief Makes the particles now see each other as part of the same membrane.
     * 
     * @param sizeX The amount of particles of the mebrane in X dircetion.
     * @param sizeY The amount of particles of the mebrane in Y dircetion.
     */
    void makeMembrane(int sizeX, int sizeY) override;

    /**
     * @brief Applys force to the particle at the specified palce in the membrane.
     * 
     * @param sizeX The amount of particles of the mebrane in X dircetion.
     * @param x The location of the particle int the mebrane in x dircetion.
     * @param y The location of the particle int the mebrane in y dircetion.
     * @param force The force that is applyed to the particle.
     */
    void applyForce(int x, int y, int sizeX, std::array<double, 3> force) override;

    /**
     * @brief Check if the particles are closer than the cut off radius.
     * 
     * @param particle1 The first particle.
     * @param particle2 The second particle.
     * 
     * @return A bool value if they are closer.
     */
    const bool inCuttofRaius(const Particle* particle1, const Particle* particle2) ;

private:
    size_t particle_count; ///< The number of particles in this container.
    const std::array<size_t, 3> cellCount; ///< Number of cells in each dimension.
    const std::array<double, 3> cellSize; ///< Size of each cell in three dimensions.
    const std::array<double, 3> size; ///< Size of the container in three dimensions.
    const double radiusSquared; ///< Cut-off radius.
    std::vector<Particle*> halo; ///< Vector containing particles outside the calculated area.
    const size_t vectorLength; ///< Total number of cells in the container.
    std::vector<std::list<Particle*>> linkedCells; ///< Vector of linked cells containing particles.
    std::array<int, 8> lastReseve; /// < The estimate the vector size in getParticlePairs peridic, the amount, that was reserved last time.
};