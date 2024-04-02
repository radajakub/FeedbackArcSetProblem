#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace co {

    class Vertex {
    public:
        int vertex;
        int cost;

        Vertex(int vertex, int cost) : vertex(vertex), cost(cost) {}

        void print(bool newLine);
    };

    class DGraph {
    public:
        // |E|
        int E;

        // |V|
        int V;

        // holds lists of adjacent vertices for easier iterations
        std::vector<std::vector<Vertex>> out_edges;
        std::vector<std::vector<Vertex>> in_edges;

        // construct a graph representation frmo a list of edges
        DGraph(std::string &path);

        // print graph information
        void print();
    };

};
#endif
