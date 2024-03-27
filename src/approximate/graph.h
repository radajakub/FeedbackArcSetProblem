#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <fstream>
#include <iomanip>
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

        void println();
    };

    class Node {
    public:
        int node;
        int cost;

        Node(int node, int cost) : node(node), cost(cost) {}

        void print(bool newLine);
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
        std::vector<std::vector<Node>> out_edges;
        std::vector<std::vector<Node>> in_edges;

        // construct a graph representation frmo a list of edges
        DGraph(std::vector<Edge> &edges);

        // print graph information
        void print();
    };

    std::vector<Edge> load_edges(std::string path);

};
#endif
