CMAKE_MINIMUM_REQUIRED(VERSION 3.15.0)

PROJECT(TestMath)

add_subdirectory(external/gtest)

SET(SRC_FILES test_vec.cpp)

add_executable(TestMath ${SRC_FILES})
target_link_libraries(TestMath PUBLIC gtest)
