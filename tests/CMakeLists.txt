cmake_minimum_required(VERSION 3.14)

add_executable(tests 
    tests.cpp
    ${PROJECT_SOURCE_DIR}/src/Particle.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticleContainer.cpp
    ${PROJECT_SOURCE_DIR}/src/Forces/Lennard_Jones_Force.cpp
    ${PROJECT_SOURCE_DIR}/src/inputFileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/particlegen/ParticleGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/FileReader.cpp
)

target_link_libraries(tests gtest_main)

target_include_directories(tests PRIVATE ../libs/spdlog-1.14.1/include)

enable_testing()

add_test(NAME tests COMMAND tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")




