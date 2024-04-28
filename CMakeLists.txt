cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -pthread")

add_executable(ea src/approximate/main.cpp src/approximate/builders.cpp src/approximate/ea.cpp src/approximate/graph.cpp src/approximate/modifiers.cpp src/approximate/state.cpp src/approximate/utils.cpp)

