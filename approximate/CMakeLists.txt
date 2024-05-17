cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp -O2 -march=native -pthread -Xclang -fopenmp")

add_executable(alns src/main.cpp src/save.cpp src/utils.cpp src/graph.cpp src/state.cpp src/exact.cpp src/alns_builders.cpp src/alns_operators.cpp src/alns_selectors.cpp src/alns_acceptors.cpp src/alns_local_search.cpp src/alns.cpp)
