#ifndef __ALNS_H__
#define __ALNS_H__

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <utility>

#include "alns_acceptors.h"
#include "alns_builders.h"
#include "alns_local_search.h"
#include "alns_operators.h"
#include "alns_selectors.h"
#include "graph.h"
#include "state.h"

const int RESTART_ITERS = 1000;

namespace co {

    class ALNS {
    private:
        std::mt19937 rng;
        std::uniform_real_distribution<double> p_dist;
        std::chrono::steady_clock::time_point deadline;

        std::vector<op> operators;

        int next_builder;
        std::uniform_int_distribution<int> builder_dist;
        std::vector<builder> builders;
        std::vector<builder> restart_builders;

        std::unique_ptr<co::select::Selector> selector;
        std::unique_ptr<co::accept::Acceptor> acceptor;

        long int iter;
        std::chrono::milliseconds prev_iter;
        std::chrono::steady_clock::time_point iter_start_time;

    public:
        ALNS(int seed, std::chrono::steady_clock::time_point deadline, int V);

        State solve(DGraph &g);

        builder choose_builder();

        void iter_start();
        void iter_stop();

        bool can_continue();
    };
};

#endif
