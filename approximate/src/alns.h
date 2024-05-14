#ifndef __ALNS_H__
#define __ALNS_H__

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <utility>

#include "alns_operators.h"
#include "alns_selectors.h"
#include "builders.h"
#include "graph.h"
#include "state.h"

const int RESTART_ITERS = 100;

namespace co {

    class ALNS {
    private:
        std::mt19937 rng;
        std::uniform_real_distribution<double> p_dist;
        std::chrono::steady_clock::time_point deadline;

        std::vector<op> operators;

        std::unique_ptr<co::select::Selector> selector;

        int iter;
        std::chrono::milliseconds prev_iter;
        std::chrono::steady_clock::time_point iter_start_time;

    public:
        ALNS(int seed, std::chrono::steady_clock::time_point deadline);

        State solve(DGraph &g);

        void iter_start();
        void iter_stop();

        bool can_continue();
    };
};

#endif
