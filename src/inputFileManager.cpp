
#include "spdlog/spdlog.h"
#include "inputFileManager.h"


void inputFileManager::mergeFile(const char *filename){

    std::fstream input_file;
    std::string tmp_string;
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out);
    while (tmp_string.empty() or tmp_string[0] == '#') {
        getline(input_file, tmp_string);
    }
    input_file.seekp(input_file.tellp()-33);

    std::ifstream mergin_file(filename);
    std::string tmp_string_merg;

    if (mergin_file.is_open()) {
        getline(mergin_file, tmp_string_merg);

        while (tmp_string_merg.empty() or tmp_string_merg[0] == '#') {
            getline(mergin_file, tmp_string_merg);
        }
    } 
    else {
        spdlog::error("Error: could not open file {}", filename);
    exit(-1);
    }
    input_file.seekp(input_file.tellp()-33);
    int num_particles = std::stoi(tmp_string_merg);
    input_file << std::stoi(tmp_string)+num_particles;    
    input_file.close();
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out|std::ios::app);
    for (int i = 0; i < num_particles; i++) {
        getline(mergin_file, tmp_string_merg);
        input_file << tmp_string_merg;
    }
    input_file.close();
}

void inputFileManager::resetFile(){
    std::fstream input_file;
    input_file.open("../input/generated-input.txt",std::ios::in|std::ios::out|std::ios::trunc);
    input_file << "# Inputfile where all used particles will be stored.\n# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles\n# there have to follow the exact quantity of spaces so that the number of chars in the line\n# adds up to 32 (not counting the \"\\n\" at the end)\n0                               \n";
    input_file.close();
}
