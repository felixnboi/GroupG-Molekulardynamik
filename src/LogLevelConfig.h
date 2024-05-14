#ifndef LOG_LEVEL_CONFIG_H
#define LOG_LEVEL_CONFIG_H

#include <spdlog/spdlog.h>

#ifdef LOG_LEVEL
    #define LOG_LEVEL_STRINGIFY(x) #x
    #define LOG_LEVEL_TO_ENUM(x) spdlog::level::##x
    #define LOG_LEVEL_ENUM LOG_LEVEL_TO_ENUM(LOG_LEVEL)
#else
    #define LOG_LEVEL_ENUM spdlog::level::info
#endif

#endif // LOG_LEVEL_CONFIG_H
