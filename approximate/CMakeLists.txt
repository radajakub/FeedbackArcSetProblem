cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -pthread")

add_executable(alns src/main.cpp src/save.cpp src/utils.cpp src/graph.cpp src/state.cpp src/exact.cpp src/builders.cpp src/alns_operators.cpp src/alns_selectors.cpp src/alns.cpp)
