#pragma once

#include <stddef.h>
#include <string>

/**
 * @class OpenMPData
 * @brief Class to hold OpenMP configuration data.
 */
class OpenMPData {
public:
    /**
     * @brief Default constructor
     */
    OpenMPData();

    /**
     * @brief Constructor with full parameters
     * 
     * @param openmp_flag Flag indicating whether OpenMP is enabled
     * @param num_threads Number of threads to be used in parallel regions
     * @param strategy OpenMP parallelization strategy
     */
    OpenMPData(bool openmp_flag, size_t num_threads, std::string strategy);

    /**
     * @brief Getter for the OpenMP flag
     * 
     * @return True if OpenMP is enabled, false otherwise
     */
    bool getOpenMPFlag() const;

    /**
     * @brief Getter for the number of threads
     * 
     * @return The number of threads used in OpenMP parallel regions
     */
    size_t getNumThreads() const;

    /**
     * @brief Getter for the OpenMP parallelization strategy
     * 
     * @return The strategy used for OpenMP parallelization
     */
    std::string getStrategy() const;

    /**
     * @brief Setter for the OpenMP flag
     * 
     * @param new_openmp_flag The new value of the OpenMP flag
     */
    void setOpenMPFlag(bool new_openmp_flag);

    /**
     * @brief Setter for the number of threads
     * 
     * @param new_num_threads The new number of threads to be used in parallel regions
     */
    void setNumThreads(size_t new_num_threads);

    /**
     * @brief Setter for the OpenMP parallelization strategy
     * 
     * @param new_strategy The new OpenMP parallelization strategy
     */
    void setStrategy(const std::string& new_strategy);

private:
    bool openmp_flag; ///< Flag indicating whether OpenMP is enabled.
    size_t num_threads; ///< Number of threads to be used in OpenMP parallel regions.
    std::string strategy; ///< OpenMP parallelization strategy.
};
