#ifndef __ALNS_OPERATORS_H__
#define __ALNS_OPERATORS_H__

#include <functional>
#include <random>
#include <utility>
#include <vector>

#include "exact.h"
#include "graph.h"
#include "state.h"

const double MAX_DESTROY_RATIO = 0.1;

namespace co {
    typedef std::vector<int> op_change;
    typedef std::function<op_change(DGraph &g, State &s, std::mt19937 &rng)> destroy_op;
    typedef std::function<void(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng)> repair_op;
    typedef std::pair<destroy_op, repair_op> op;

    namespace destroy {
        // remove a random vertex
        op_change random(DGraph &g, State &s, std::mt19937 &rng);
        // remove MAX_DESTROY_RATIO of vertices
        op_change random_multiple(DGraph &g, State &s, std::mt19937 &rng);
        // remove MAX_DESTROY_RATIO of consecutive vertices
        op_change random_range(DGraph &g, State &s, std::mt19937 &rng);
        op_change random_range_sorted(DGraph &g, State &s, std::mt19937 &rng);
        // remove a random vertex and all backward adjacent vertices
        op_change backward_adjacent(DGraph &g, State &s, std::mt19937 &rng);
        op_change adjacent(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly_multiple(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly_adjacent(DGraph &g, State &s, std::mt19937 &rng);
        op_change high_degree(DGraph &g, State &s, std::mt19937 &rng);
        op_change mostly_backwards(DGraph &g, State &s, std::mt19937 &rng);
        op_change more_incoming(DGraph &g, State &s, std::mt19937 &rng);
    };

    namespace repair {
        void random(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng);
        void greedy(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng);
    };

};

#endif
