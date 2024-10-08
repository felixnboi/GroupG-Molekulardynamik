#include "inputFileManager.h"


void inputFileManager::mergeFile(const char *filename1, const char *filename2){
    spdlog::info("Starting to merge file: {}", filename2);

    std::fstream input_file;
    std::string tmp_string;

    input_file.open(filename1,std::ios::in|std::ios::out);
    std::streampos current = input_file.tellp();
    if (!input_file.is_open()) {
        spdlog::error("Failed to open generated input file");
        exit(-1);
    }
    while (tmp_string.empty() or tmp_string[0] == '#') {
        current = input_file.tellp();
        getline(input_file, tmp_string);
    }
    input_file.seekp(current);

    std::fstream mergin_file(filename2);
    std::string tmp_string_merg;

    if (mergin_file.is_open()) {
        getline(mergin_file, tmp_string_merg);

        while (tmp_string_merg.empty() or tmp_string_merg[0] == '#') {
            current = mergin_file.tellp();
            getline(mergin_file, tmp_string_merg);
        }
    }else {
        spdlog::error("Failed to open file {}", filename2);
        exit(-1);
    }
    int num_particles = std::stoi(tmp_string_merg);
    input_file << std::stoi(tmp_string)+num_particles;    
    input_file.close();
    input_file.open(filename1,std::ios::in|std::ios::out|std::ios::app);
    if (!input_file.is_open()) {
        spdlog::error("Failed to reopen generated input file");
        exit(-1);
    }

    for (int i = 0; i < num_particles; i++) {
        getline(mergin_file, tmp_string_merg);
        input_file << tmp_string_merg << "\n";
    }
    input_file.close();
    spdlog::info("Merged file: {}", filename2);
}

void inputFileManager::resetFile(const char *filename){
    spdlog::info("Resetting generated input file");
    std::fstream input_file;
    input_file.open(filename,std::ios::in|std::ios::out|std::ios::trunc);
    if (!input_file.is_open()) {
        spdlog::error(filename);
        exit(-1);
    }
    input_file << "# Inputfile where all used particles will be stored.\n# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles\n# there have to follow the exact quantity of spaces so that the number of chars in the line\n# adds up to 32 (not counting the \"\\n\" at the end)\n0                               \n";
    input_file.close();
}
