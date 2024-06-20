cmake_minimum_required(VERSION 3.14)

add_executable(tests
    xmltests.cpp
    tests.cpp
    ${PROJECT_SOURCE_DIR}/src/data/Particle.cpp
    ${PROJECT_SOURCE_DIR}/src/particlecontainers/ParticleContainer.h
    ${PROJECT_SOURCE_DIR}/src/particlecontainers/ParticleContainerOld.cpp
    ${PROJECT_SOURCE_DIR}/src/particlecontainers/ParticleContainerLinkedCell.cpp
    ${PROJECT_SOURCE_DIR}/src/forces/Lennard_Jones_Force.cpp
    ${PROJECT_SOURCE_DIR}/src/inputFileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/particlegen/ParticleGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/io/input/FileReader.cpp
    ${PROJECT_SOURCE_DIR}/src/io/input/XMLReader.cpp
    ${PROJECT_SOURCE_DIR}/src/io/input/XMLFormat.cpp
    ${PROJECT_SOURCE_DIR}/src/data/Cuboid.cpp    
    ${PROJECT_SOURCE_DIR}/src/data/Disc.cpp 
    ${PROJECT_SOURCE_DIR}/src/data/SimData.cpp
    ${PROJECT_SOURCE_DIR}/src/data/ThermostatData.cpp
    ${PROJECT_SOURCE_DIR}/src/data/CheckpointData.cpp
    ${PROJECT_SOURCE_DIR}/src/thermostat/Thermostat.cpp
)

target_link_libraries(tests 
    PUBLIC
        gtest_main 
        xerces-c
)

target_include_directories(tests 
    PRIVATE 
        ../libs/spdlog-1.14.1/include
)

enable_testing()

add_test(NAME tests COMMAND tests)

set_tests_properties(tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")




