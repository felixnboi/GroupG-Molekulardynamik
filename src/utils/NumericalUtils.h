#pragma once 

#include <string>

/**
 * @brief Checks if the given string is a double 
 */ 
inline bool isDouble(char* str) {
    try {
        std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * @brief Checks if the given string is an unsigned int 
 */ 
inline bool isUnsignedInt(char* str) {
    try {
        unsigned long value = std::stoul(str);
        return value <= std::numeric_limits<unsigned int>::max();
    } catch (...) {
        return false;
    }
}

/**
 * @brief Checks if the given string is an int 
 */ 
inline bool isInteger(char* str) {
    try {
        std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}