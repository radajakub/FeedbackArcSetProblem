#ifndef __ALNS_H__
#define __ALNS_H__

#include <chrono>
#include <iostream>
#include <random>

#include "builders.h"
#include "graph.h"
#include "state.h"

namespace co {

    class ALNS {
    private:
        std::mt19937 rng;
        std::uniform_real_distribution<double> p_dist;
        std::chrono::steady_clock::time_point deadline;

    public:
        ALNS(int seed, std::chrono::steady_clock::time_point deadline);

        State solve(DGraph &g);
    };

    // delete operators
    // delete a random vertex (set it to -1)
    // delete a most costly vertex (with most backwards edges)
    // delete a most costly subsequence ?? or just a random subsequence

    // insert operators
    // random greedy insert on a best position
    // insert on a random position

    // eps-greedy or ucb bandit for operator selection

    // acceptance criterions for the new solution
    // only improvement
    // simulated annealing (expenential decay of probability)

    // maybe add random restarts??
};

#endif
