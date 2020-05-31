cmake_minimum_required(VERSION 3.12)
project(sem4_cousework_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

add_subdirectory(google_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(sem4_cousework_tests tests.cpp ../flow_network.cpp)
target_link_libraries(sem4_cousework_tests gtest gtest_main)

