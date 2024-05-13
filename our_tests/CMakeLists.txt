# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Include directories for Google Test headers
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add test_example.cpp as a test target
#add_executable(tests firsttest.cpp)

# Link Google Test to your test executable
#target_link_libraries(tests gtest gtest_main)




enable_testing()

include_directories(our_googletest/googletest/googletest/include)

include(GoogleTest)
#add_executable(teststotest firsttest.cpp)

# Link Google Test to your test executable
#target_link_libraries(teststotest gtest gtest_main)
#gtest_discover_tests(teststotest)