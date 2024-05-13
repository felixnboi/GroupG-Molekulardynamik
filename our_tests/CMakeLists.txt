# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)
project(ourtests)
set(CMAKE_CXX_STANDARD 14)

enable_testing()
include(GoogleTest)

include_directories(${PROJECT_SOURCE_DIR}/../src)

include_directories(${PROJECT_SOURCE_DIR}/our_googletest/googletest/googletest/include)

###################add_executable(ParticleContainerTest firsttest.cpp)
add_executable(ParticleContainerTest 
our_tests/firsttest.cpp
    ${PROJECT_SOURCE_DIR}/../src/Particle.cpp
    ${PROJECT_SOURCE_DIR}/../src/ParticleContainer.cpp
)

target_include_directories(ParticleContainerTest PRIVATE ../src)
target_link_libraries(ParticleContainerTest gtest gtest_main)

# Link Google Test to your test executable
gtest_discover_tests(ParticleContainerTest)
