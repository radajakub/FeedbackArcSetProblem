cmake_minimum_required(VERSION 3.10)
project(SmartCity)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/opt/libomp/include -O2 -march=native -pthread -Xclang -fopenmp")

link_directories(/opt/homebrew/opt/libomp/lib)
add_link_options(-lomp)

add_executable(alns src/main.cpp src/save.cpp src/utils.cpp src/graph.cpp src/state.cpp src/alns_builders.cpp src/alns_operators.cpp src/alns_selectors.cpp src/alns_acceptors.cpp src/alns.cpp)
