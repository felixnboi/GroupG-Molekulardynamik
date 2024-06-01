#include "inputFileManager.h"


void inputFileManager::mergeFile(const char *filename1, const char *filename2){
    spdlog::info("Starting to merge file: {}", filename2);
    std::fstream input_file(filename1, std::ios::in | std::ios::out);
     if (!input_file.is_open()) {
        spdlog::error("Failed to open generated input file");
        exit(-1);
    }

    std::string tmp_string;
    std::streampos current;

    while (getline(input_file, tmp_string) && (tmp_string.empty() || tmp_string[0] == '#')) {
        current = input_file.tellg();
    }

    int initial_num_particles;
    input_file.seekg(current);
    input_file >> initial_num_particles;
    //
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip to the end of the line

    //std::fstream mergin_file(filename2);
    std::fstream merging_file(filename2, std::ios::in);
    if (!merging_file.is_open()) {
        spdlog::error("Failed to open file {}", filename2);
        exit(-1);
    }
    
    std::string tmp_string_merg;
    int new_num_particles = 0;
    while (getline(merging_file, tmp_string_merg) && (tmp_string_merg.empty() || tmp_string_merg[0] == '#'));

    merging_file.seekg(-tmp_string_merg.length(), std::ios::cur);
    merging_file >> new_num_particles;
    //
     merging_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip to the end of the line

    input_file.seekp(current);
    input_file << initial_num_particles + new_num_particles << std::endl;

    input_file.seekp(0, std::ios::end);

    while (getline(merging_file, tmp_string_merg)) {
        if (!tmp_string_merg.empty() && tmp_string_merg[0] != '#') {
            input_file << tmp_string_merg << '\n';
        }
    }
    input_file.close();
    merging_file.close();
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
