cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -pthread")

add_executable(ea src/main.cpp src/builders.cpp src/ea.cpp src/graph.cpp src/modifiers.cpp src/state.cpp src/utils.cpp)

