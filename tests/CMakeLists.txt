cmake_minimum_required(VERSION 3.14)

add_executable(tests 
    tests.cpp
    ${PROJECT_SOURCE_DIR}/src/Particle.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticleContainer.cpp
    ${PROJECT_SOURCE_DIR}/src/Forces/Lenard_Jones_Force.cpp
    ${PROJECT_SOURCE_DIR}/src/inputFileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/particlegen/ParticleGenerator.cpp
    ${PROJECT_SOURCE_DIR}/src/FileReader.cpp
)

target_link_libraries(tests gtest_main)

target_include_directories(tests PRIVATE ../spdlog-1.14.1/include)

enable_testing()

add_test(NAME tests COMMAND tests)

add_subdirectory(googletest-1.14.0)


