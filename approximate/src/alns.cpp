#include "alns.h"

co::ALNS::ALNS(int seed, std::chrono::steady_clock::time_point deadline) {
    if (seed == -1) {
        std::random_device rd;
        this->rng = std::mt19937(rd());
    } else {
        this->rng = std::mt19937(seed);
    }

    this->deadline = deadline;
    this->p_dist = std::uniform_real_distribution<double>(0, 1);
}

co::State co::ALNS::solve(co::DGraph &g) {
    co::State s = co::builders::out_cost(g);
    s.evaluate_full(g);

    return s;
}
