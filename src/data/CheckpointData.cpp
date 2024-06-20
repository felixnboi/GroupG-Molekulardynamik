#include "CheckpointData.h"

CheckpointData::CheckpointData(bool checkpoint_flag, bool checkpoint_file_flag, std::string checkpoint_file, bool merge_file_flag, 
std::string merge_file)
    : checkpoint_flag(checkpoint_flag), 
      checkpoint_file_flag(checkpoint_file_flag), 
      checkpoint_file(checkpoint_file), 
      merge_file_flag(merge_file_flag), 
      merge_file(merge_file) {}

CheckpointData::~CheckpointData() {}

bool CheckpointData::getCheckpointFlag() const { return checkpoint_flag; }
bool CheckpointData::getCheckpointFileFlag() const { return checkpoint_file_flag; }
std::string CheckpointData::getCheckpointFile() const { return checkpoint_file; }
bool CheckpointData::getMergeFileFlag() const { return merge_file_flag; }
std::string CheckpointData::getMergeFile() const { return merge_file; }

void CheckpointData::setCheckpointFlag(bool new_checkpoint_flag) { checkpoint_flag = new_checkpoint_flag; }
void CheckpointData::setCheckpointFileFlag(bool new_checkpoint_file_flag) { checkpoint_file_flag = new_checkpoint_file_flag; }
void CheckpointData::setCheckpointFile(std::string new_checkpoint_file) { checkpoint_file = new_checkpoint_file; }
void CheckpointData::setMergeFileFlag(bool new_merge_file_flag) { merge_file_flag = new_merge_file_flag; }
void CheckpointData::setMergeFile(std::string new_merge_file) { merge_file = new_merge_file; }
