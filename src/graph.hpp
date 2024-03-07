#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace co
{

    class DGraph
    {
    private:
        // |E|
        int E;

        // |V|
        int V;

        // holds lists of adjacent vertices for easier iterations
        std::vector<std::vector<int>> adjacency_list;

        // holds costs in a matrix
        std::vector<std::vector<int>> adjacency_matrix;

    public:
        DGraph(std::vector<std::vector<int>> edges);
        void print();
    };

    std::vector<std::vector<int>> load_edges(std::string path);

};
#endif
