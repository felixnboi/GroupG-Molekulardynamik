cmake_minimum_required(VERSION 3.14)

add_executable(tests
    tests.cpp
    ${PROJECT_SOURCE_DIR}/src/data/Particle.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticleContainers/ParticleContainer.h
    ${PROJECT_SOURCE_DIR}/src/ParticleContainers/ParticleContainerOld.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticleContainers/ParticleContainerLinkedCell.cpp
    ${PROJECT_SOURCE_DIR}/src/Forces/Lennard_Jones_Force.cpp
    ${PROJECT_SOURCE_DIR}/src/inputFileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/particlegen/ParticleGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/io/input/FileReader.cpp
    ${PROJECT_SOURCE_DIR}/src/data/Cuboid.cpp    
    ${PROJECT_SOURCE_DIR}/src/data/Disc.cpp    
)

target_link_libraries(tests gtest_main)

target_include_directories(tests PRIVATE ../libs/spdlog-1.14.1/include)

enable_testing()

add_test(NAME tests COMMAND tests)

set_tests_properties(tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")




