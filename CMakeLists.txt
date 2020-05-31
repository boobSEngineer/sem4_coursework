cmake_minimum_required(VERSION 3.12)
project(sem4_cousework)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(sem4_cousework main.cpp flow_network.cpp)
