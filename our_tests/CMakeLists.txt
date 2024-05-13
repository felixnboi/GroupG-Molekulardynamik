# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)
project(ourtests)
set(CMAKE_CXX_STANDARD 14)

enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/../src)
add_executable(ParticleContainerTest ParticleContainerTest.cpp)

include_directories(our_googletest/googletest/googletest/include)


include(GoogleTest)
#add_executable(teststotest firsttest.cpp)

# Link Google Test to your test executable
#target_link_libraries(teststotest gtest gtest_main)
#gtest_discover_tests(teststotest)