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

    class Edge {
    public:
        int source;
        int target;
        int cost;

        Edge(int source, int target, int cost) : source(source), target(target), cost(cost) {}
    };

    class DGraph {
    public:
        // |E|
        int E;

        // |V|
        int V;

        std::vector<Edge> edges;

        // holds lists of adjacent vertices for easier iterations
        std::vector<std::vector<Vertex>> out_edges;
        std::vector<int> out_degrees;
        std::vector<std::vector<Vertex>> in_edges;
        std::vector<int> in_degrees;

        // construct a graph representation frmo a list of edges
        DGraph(std::string &path);

        // print graph information
        void print();
    };

};
#endif
