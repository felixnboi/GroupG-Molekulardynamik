cmake_minimum_required(VERSION 3.14)

add_executable(tests 
    tests.cpp
    ${PROJECT_SOURCE_DIR}/src/Particle.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticleContainer.cpp
)

target_link_libraries(tests gtest_main)

enable_testing()

add_test(NAME tests COMMAND tests)

add_subdirectory(googletest-1.14.0)

