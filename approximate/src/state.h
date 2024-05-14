#ifndef __STATE_H__
#define __STATE_H__

#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
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

        // bool indicating if every state has assigned ordering
        bool is_full;

        // topological ordering of vertices
        // ordering[v] = order for all v in V
        // i.e. permutation 0 3 1 2 is represented as 0 2 3 1
        std::vector<int> ordering;

        // construct empty solution
        State(int V);

        // remove vertex and adjust cost
        void remove_vertex(int v, DGraph &g);

        // place vertex at position and adjust cost
        void place_vertex(int v, int pos, DGraph &g);

        std::vector<int> to_vertices();
        void from_vertices(std::vector<int> &vertices);

        // just set the order of the vertex
        void set_order(int v, int pos);
        void set_order(std::vector<std::pair<int, int>> &indexed_vector);

        // full evaluation over the whole solution
        void evaluate_full(DGraph &g);

        void print(std::string prefix);

        void save_solution(DGraph &g, std::ofstream &f);
    };

    class IncrementalState {
    public:
        int V;
        int cost;

        int placed;

        std::vector<int> ordering;

        IncrementalState(int V);

        void place_vertex(int vertex, DGraph &g);

        int cheapest_cost(DGraph &g, std::vector<int> &vertices);

        std::vector<int> to_vertices();
        State to_state();

        void print_val();
        void print_ordering();
        void print_vertices();
    };

};

#endif
