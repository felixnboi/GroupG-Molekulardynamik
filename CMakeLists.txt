cmake_minimum_required(VERSION 3.14)

# define project name, version
project(PSEMolDyn_GroupG)

find_package(XercesC REQUIRED)

include_directories(${XercesC_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd)

# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# collect all cpp files
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

set(EXCLUDE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/particlegen/ParticleGen.cpp"
)

list(REMOVE_ITEM MY_SRC ${EXCLUDE_FILES})

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim
        PRIVATE
            cxx_std_17
)

target_include_directories(MolSim
        PUBLIC
            ${XercesC_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/spdlog-1.14.1
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
            ${XercesC_LIBRARIES}
        PRIVATE
            spdlog::spdlog
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(MolSim
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            # disable some spam warnings for icpc...
            $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#include doxygen module
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/doxygen.cmake)

#this part is for the tests
add_subdirectory(tests)
enable_testing()
include(CTest)

# Add the sanitize address flag and enable profiling
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -pg")

add_subdirectory(libs/spdlog-1.14.1)
add_subdirectory(libs/googletest-1.14.0)

###############################################################################################################################

#This part is for our Particle Generator

add_executable(ParticleGenerator 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/inputFileManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Particle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/particlegen/ParticleGenerator.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/particlegen/ParticleGen.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Cuboid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/Disc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/SimData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/ThermostatData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/CheckpointData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/io/input/XMLReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/io/input/XMLFormat.cpp
)

target_compile_features(ParticleGenerator
        PRIVATE
            cxx_std_17
)

target_include_directories(ParticleGenerator
        PUBLIC
            ${XercesC_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/particlegen
)

target_link_libraries(ParticleGenerator
        # stuff that is used in headers and source files
        PUBLIC
            ${XercesC_LIBRARIES}
        PRIVATE
            spdlog::spdlog
)

# activate all compiler warnings. Clean up your code :P
# depending on the compiler different flags are used
target_compile_options(ParticleGenerator
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            # disable some spam warnings for icpc...
            $<$<CXX_COMPILER_ID:Intel>:-w3 -wd383,981,1418,1572,2259>
)

######################################################################################################################