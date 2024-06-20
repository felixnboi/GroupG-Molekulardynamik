#pragma once

#include <string>

/**
 * @class CheckpointData
 * @brief Class to handle checkpoint and merge file data.
 */
class CheckpointData {
public:
    /**
     * @brief Constructor to initialize CheckpointData.
     * 
     * @param checkpoint_flag Flag indicating if checkpointing is used.
     * @param checkpoint_file_flag Flag indicating if checkpoint file is used.
     * @param checkpoint_file Path to the checkpoint file.
     * @param merge_file_flag Flag indicating if merge file is used.
     * @param merge_file Path to the merge file.
     */
    CheckpointData(bool checkpoint_flag, bool checkpoint_file_flag, std::string checkpoint_file, bool merge_file_flag, 
    std::string merge_file);

    /**
     * @brief Destructor for CheckpointData.
     */
    ~CheckpointData();

    /**
     * @brief Getter for the checkpoint flag.
     * 
     * @return The checkpoint flag.
     */
    bool getCheckpointFlag() const;

    /**
     * @brief Getter for the checkpoint file flag.
     * 
     * @return The checkpoint file flag.
     */
    bool getCheckpointFileFlag() const;

    /**
     * @brief Getter for the path to the checkpoint file.
     * 
     * @return The path to the checkpoint file.
     */
    std::string getCheckpointFile() const;

    /**
     * @brief Getter for the merge file flag.
     * 
     * @return True if the merge file is present, false otherwise.
     */
    bool getMergeFileFlag() const;

    /**
     * @brief Getter for the path to the merge file.
     * 
     * @return The merge file flag.
     */
    std::string getMergeFile() const;

    /**
     * @brief Setter for the checkpoint flag.
     * 
     * @param new_checkpoint_flag The new value for the checkpoint flag.
     */
    void setCheckpointFlag(bool new_checkpoint_flag);

    /**
     * @brief Setter for the checkpoint file flag.
     * 
     * @param new_checkpoint_file_flag The new value for the checkpoint file flag.
     */
    void setCheckpointFileFlag(bool new_checkpoint_file_flag);

    /**
     * @brief Setter for the path to the checkpoint file.
     * 
     * @param new_checkpoint_file The new path to the checkpoint file.
     */
    void setCheckpointFile(std::string new_checkpoint_file);

    /**
     * @brief Setter for the merge file flag.
     * 
     * @param new_merge_file_flag The new value for the merge file flag.
     */
    void setMergeFileFlag(bool new_merge_file_flag);

    /**
     * @brief Setter for the path to the merge file.
     * 
     * @param new_merge_file The new path to the merge file.
     */
    void setMergeFile(std::string new_merge_file);

private:
    bool checkpoint_flag; ///<Flag indicating if checkpointing should be used or not.
    bool checkpoint_file_flag; ///<Flag indicating if checkpoint file is used.
    std::string checkpoint_file; ///< Path to the checkpoint file.
    bool merge_file_flag; ///< Flag indicating if merge file is used.
    std::string merge_file; ///< Path to the merge file.
};
