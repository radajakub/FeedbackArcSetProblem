#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace co {

    // representation for adjacency list with costs
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

        void print();
    };

    class DGraph {
    public:
        // |E|
        int E;

        // |V|
        int V;

        // list of all edges
        std::vector<Edge> edges;

        // lists of adjacent vertices for easier iterations
        std::vector<std::vector<Vertex>> out_edges;
        std::vector<std::vector<Vertex>> in_edges;

        // list of degrees for each vertex
        std::vector<int> out_degrees;
        std::vector<int> in_degrees;

        // list of costs for each vertex
        std::vector<int> out_costs;
        std::vector<int> in_costs;

        // construct a graph representation frmo a list of edges
        DGraph(std::string &path);

        // print graph information
        void print();
    };

};
#endif
