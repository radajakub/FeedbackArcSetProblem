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

        int value;

        // topological ordering of vertices
        std::vector<int> order;

        // construct empty solution
        State(int V);

        void set_order(int vertex, int order);

        // full evaluation over the whole solution
        void evaluate_full(DGraph &g);

        void evaluate_incremental(DGraph &g, co::State &original, std::vector<int> &changed);

        void save_solution(DGraph &g, std::string &path);

        void print_val();
        void println(DGraph &g);
    };
};

#endif
