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
        // i.e. permutation 0 3 1 2 is represented as 0 2 3 1
        std::vector<int> ordering;

        // construct empty solution
        State(int V);

        // set order of a vertex
        void set_order(int vertex, int order);
        void set_order(std::vector<std::pair<int, int>> &indexed_vector);

        // get the vertex permutation instead of the order
        std::vector<int> to_vertices();
        // save the vertex permutation as ordering for faster computation
        void set_vertices(std::vector<int> &vertices);

        // full evaluation over the whole solution
        void evaluate_full(DGraph &g);
        // adjusts the costs only for the vertices in changed
        void evaluate_incremental(DGraph &g, co::State &original, std::vector<int> &changed);

        void print_val();
        void print_ordering();
        void print_vertices();

        void print_solution(DGraph &g);
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

        int most_expensive_cost(DGraph &g, std::vector<int> &vertices);

        std::vector<int> to_vertices();
        State to_state();

        void print_val();
        void print_ordering();
        void print_vertices();
    };

};

#endif
