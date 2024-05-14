#include "inputFileMerger.h"

void mergeWithImputFile(char *filename){
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
    std::cout << "Error: could not open file " << filename << std::endl;
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