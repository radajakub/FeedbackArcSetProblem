cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -pthread")

add_executable(solve src/main.cpp src/utils.cpp src/graph.cpp src/state.cpp src/exact.cpp src/builders.cpp src/solvers.cpp src/modifiers.cpp)
