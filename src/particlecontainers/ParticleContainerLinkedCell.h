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
     * @param startegy The value deciding which parallelization strategy is used.
     */
    ParticleContainerLinkedCell(double sizeX, double sizeY, double sizeZ, double radius, size_t startegy);

    /**
     * @brief Destructor for ParticleContainerLinkedCell.
     */
    ~ParticleContainerLinkedCell();

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
     * @brief Getter for the pairs of particles within the interaction radius. (Ingnores periodic boundery)
     * 
     * @return A vector of these pairs of particles.
     */
    std::vector<std::pair<Particle* const, Particle* const>> getParticlePairs() override;

    /**
     * @brief Gets the pairs of particles that could interact through EVERY periodic boundery where the flag is set and only those. 
     * @brief (Doesn't always check if they are within the cut off radius).
     * 
     * @param pFlag Array indicating for which dimensions particles should interact through the periodic boundaries.
     * 
     * @return A vector of these pairs of particles.
     */
    std::vector<std::pair<Particle* const, Particle* const>> getParticlePairsPeriodic(std::array<bool, 3> pFlag);

    /**
     * @brief Helper method of getParticlePairsPeriodic. Adds the particle pairs to particlePairs, as discribed in getParticlePairsPeriodic, with the exeptiom
     * @brief that the first particle of each pair also is in the cell with index i.
     * 
     * @param particlePairs The vector to which the particle pairs should be added.
     * @param i The index of the cell in linkedCells, in which the first particle of each pair must be.
     * @param pFlag Array indicating for which dimensions particles should interact through the periodic boundaries.
     * 
     * @return The amount of particles added to particle pairs.
     */
    inline size_t getParticlePairsPeridicOneCell(std::vector<std::pair<Particle* const, Particle* const>>& particlePairs, size_t i, std::array<bool, 3> pFlag);

    /**
     * @brief Helper method of getParticlePairsPeriodic. Adds the particle pairs to of particle_i to particlePairs, with the same restictions as in getParticlePairsPeriodic.
     * 
     * @param particlePairs The vector to which the particle pairs should be added.
     * @param i The index of the cell in linkedCells, in which particle_i currently is.
     * @param particle_i The first particle of the pairs.
     * @param nbrCount The amount of neighbour cells we want to look at. (Only half of the possible ones because of Newtons third law)
     * @param nbrs The neighbour cells we want to look at. Only the elements with index < nbrCount are actual ones.
     * @param pFlag Array indicating for which dimensions particles should interact through the periodic boundaries.
     * 
     * @return The amount of particles added to particle pairs.
     */
    inline size_t getParticlePairsPeriodicOneParticle(std::vector<std::pair<Particle* const, Particle* const>>& particlePairs, size_t i,  
    std::vector<Particle*>::iterator particle_i, size_t nbrCount, const std::array<size_t,13>& nbrs, std::array<bool, 3> pFlag);

    /**
     * @brief Check if the particles are closer than the cut off radius.
     * 
     * @param distance The location of one of the particles substracted by the location of the other.
     * 
     * @return A bool value if they are closer.
     */
    inline bool inCuttofRaius(std::array<double, 3UL> distance);

    /**
     * @brief Updates the locations of particles considering outflow and periodic boundary conditions.
     * 
     * @param outflowflag Array indicating which faces of the simulation domain have outflow boundaries.
     * @param periodicflag Array indicating which dimensions have periodic boundaries.
     */
    void updateLoctions(std::array<bool,6> outflowflag, std::array<bool,3> periodicflag);

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
     * @brief Calculates the what the one dimesional linked cell index would be in 3 dimesions.
     * 
     * @param index The one dimesional index of a cell in linkedCells.
     * 
     * @return What that index would be if linkedCells were three dimesional.
     */
    std::array<size_t, 3> index1DTo3D(size_t index);

    /**
     * @brief Calculates the index in linkedCells of a particle with this position.
     * 
     * @param position The position of the particle
     * 
     * @return The index it would have in linked cells.
     */
    size_t positionToIndex(std::array<double, 3> position);

    /**
     * @brief Gets the particles located at the boundary of the container.
     * 
     * @return A vector of shared pointers to the particles at the boundary.
     */
    std::vector<Particle*> getBoundary();

    /**
     * @brief Gets the particles located at the the specified boundaries of the container.
     * 
     * @param bounderies The bounderies at which the particles are that interest us.
     * 
     * @return A vector of pointers to the particles at the specified boundaries.
     */
    std::vector<Particle*> getBoundaries(std::array<bool, 6> boundaries);

    /**
     * @brief Gets the particles located in the halo.
     * 
     * @return A vector of shared pointers to the particles in the halo.
     */
    std::vector<Particle*> getHalo();
    
    /**
    * @brief Getter for the number of cells in each dimension.
    * 
    * @return The number of cells in each dimension as an array.
    */
    const std::array<size_t, 3> getCellCount();

    /**
     * @brief Getter for the squared interaction radius.
     * 
    * @return The squared interaction radius.
    */
    const double getRadiusSquared();

    /**
     * @brief Gets the particles in the container.
     * 
     * @return A reference to the vector of particles.
     */
    const std::vector<Particle*>& getParticles() override;

    /**
     * @brief Returns the number of particles in this particle container.
     * 
     * @return The number of particles in this container.
     */
    size_t getParticleCount() override;

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

private:
    size_t particle_count; ///< The number of particles in this container.
    std::vector<std::vector<Particle*>> linkedCells; ///< Vector of linked cells containing particles.
    std::array<double, 3> size; ///< Size of the container in three dimensions.
    std::array<double, 3> cellSize; ///< Size of each cell in three dimensions.
    std::array<size_t, 3> cellCount; ///< Number of cells in each dimension.
    double radiusSquared; ///< Cut-off radius.
    std::vector<Particle*> halo; ///< Vector containing particles outside the calculated area.
    size_t cellCountTotal; ///< Total number of cells in the container.
    std::array<size_t, 8> lastReserve; /// < The estimate the vector size in getParticlePairs peridic, the amount, that was reserved last time.
    size_t strategy; ///< Value used for choosing the parallelization strategy.
    omp_lock_t lock;
};