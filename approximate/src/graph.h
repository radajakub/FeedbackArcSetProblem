#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

#include "utils.h"

namespace co {
    class Edge {
    public:
        int source;
        int target;
        int cost;

        Edge(int source, int target, int cost) : source(source), target(target), cost(cost) {}

        void print();
    };

    class Vertex {
    public:
        int vertex;
        int cost;

        Vertex(int vertex, int cost) : vertex(vertex), cost(cost) {}

        void print();
    };

    class DGraph {
    public:
        int V;
        int E;

        std::vector<int> vertex_map;
        std::vector<Edge> edges;

        // list of out and in edges for each vertex
        std::vector<std::vector<Vertex>> out_edges;
        std::vector<std::vector<Vertex>> in_edges;

        // list of degrees for each vertex
        std::vector<int> out_degrees;
        std::vector<int> in_degrees;

        // list of costs for each vertex
        std::vector<int> out_costs;
        std::vector<int> in_costs;

        DGraph(int V, std::vector<Edge> &edges, std::vector<int> &vertex_map);

        std::vector<std::pair<int, int>> out_cost_vertices();
        std::vector<std::pair<int, int>> in_cost_vertices();
        std::vector<std::pair<int, int>> out_degree_vertices();
        std::vector<std::pair<int, int>> in_degree_vertices();

        void print();
    };

    class InputGraph {
    public:
        int V;
        int E;

        std::vector<Edge> edges;

        InputGraph(std::string &path);

        std::vector<DGraph> condense();
        void strong_connect(int v, int &index, std::stack<int> &stack, std::vector<int> &indices, std::vector<int> &lowlinks, std::vector<bool> &onstacks, std::vector<co::DGraph> &subgraphs);

        void print();
    };

};

#endif
