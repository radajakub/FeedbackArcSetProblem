#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace co {

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

        // holds list of edges
        std::vector<Edge> edges;

        // holds lists of adjacent vertices for easier iterations
        std::vector<std::vector<int>> adjacency_list;

        // holds costs in a matrix
        std::vector<std::vector<int>> adjacency_matrix;

        // construct a graph representation frmo a list of edges
        DGraph(std::vector<Edge> &edges);

        std::vector<std::vector<int>> build_adjacency_list(std::vector<Edge> &edges);

        // print graph information
        void print();
    };

    std::vector<Edge> load_edges(std::string path);

    void print_vector(std::vector<int> &v);
    void print_matrix(std::vector<std::vector<int>> &m);

};
#endif
