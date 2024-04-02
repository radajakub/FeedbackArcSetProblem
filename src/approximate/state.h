#ifndef __STATE_H__
#define __STATE_H__

#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>
#include <vector>

#include "graph.h"

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

        void random_init(std::default_random_engine &rng);

        // full evaluation over the whole solution
        void evaluate_full(DGraph &g);

        void save_solution(co::DGraph &g, std::string &path);

        void println(co::DGraph &g);
    };
};

#endif
