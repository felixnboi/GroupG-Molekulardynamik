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
    size_t getParticleCount() override;

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
    void addParticle(const std::shared_ptr<Particle> particle) override;

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
    const std::vector<std::shared_ptr<Particle>>& getParticles() override;

    /**
     * @brief Gets the size of the container.
     * 
     * @return The size of the container as an array of three doubles.
     */
    const std::array<double, 3> getSize();

    /**
     * @brief Getter for the size of each cell.
     * 
     * @return The size of each cell as an array of three doubles.
     */
    const std::array<double, 3> getCellSize();    
    
    /**
    * @brief Getter for the number of cells in each dimension.
    * 
    * @return The number of cells in each dimension as an array.
    */
    const std::array<size_t, 3> getCellCount();

    /**
     * @brief Getter for the interaction radius.
     * 
    * @return The interaction radius.
    */
    const double getRadius();

    /**
     * @brief Getter for the pairs of particles within the interaction radius.
     * 
     * @return A vector of pairs of particles.
     */
    std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairs() override;

    /**
    * @brief Gets the pairs of particles that are within the interaction radius, considering periodic boundaries.
    * 
    * @param pFlag Array indicating which dimensions have periodic boundaries.
    * 
    * @return A vector of pairs of particles that are within the interaction radius, considering periodic boundaries.
    */
    std::vector<std::array<std::shared_ptr<Particle>,2>> getParticlePairsPeriodic(std::array<bool, 3> pFlag);

    /**
     * @brief Gets the particles located at the boundary of the container.
     * 
     * @return A vector of shared pointers to the particles at the boundary.
     */
    std::vector<std::shared_ptr<Particle>> getBoundary();

    /**
     * @brief Gets the particles located in the halo.
     * 
     * @return A vector of shared pointers to the particles in the halo.
     */
    std::vector<std::shared_ptr<Particle>> getHalo();

private:
    size_t particle_count; ///< The number of particles in this container.
    std::unique_ptr<std::list<std::shared_ptr<Particle>>[]> linkedCells; ///< Array of linked cells containing particles.
    std::array<double, 3> size; ///< Size of the container in three dimensions.
    std::array<double, 3> cellSize; ///< Size of each cell in three dimensions.
    std::array<size_t, 3> cellCount; ///< Number of cells in each dimension.
    double radius; ///< Cut-off radius.
    std::vector<std::shared_ptr<Particle>> halo; ///< Vector containing particles outside the calculated area.
    size_t arrayLength; ///< Total number of cells in the container.
};