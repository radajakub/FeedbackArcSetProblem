#ifndef __STATE_H__
#define __STATE_H__

#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>
#include <vector>

#include "graph.h"
#include "utils.h"

namespace co {
    class State {
    public:
        // number of vertices
        int V;

        // total cost of the current ordering
        int cost;

        // topological ordering of vertices
        // ordering[v] = order for all v in V
        std::vector<int> ordering;

        // construct empty solution
        State(int V);

        // set order of a vertex
        void set_order(int vertex, int order);
        void set_order(std::vector<std::pair<int, int>> &indexed_vector);

        // full evaluation over the whole solution
        void evaluate_full(DGraph &g);
        // adjusts the costs only for the vertices in changed
        void evaluate_incremental(DGraph &g, co::State &original, std::vector<int> &changed);

        // transform list of orders saved in this object to a list of vertices
        std::vector<int> get_vertex_permutation();
        // save ordered vertices to inner ordering representation
        void update_ordering(std::vector<int> &vertex_permutation);

        void save_solution(DGraph &g, std::string &path);
        void print_val();
        void println(DGraph &g);
    };
};

#endif
