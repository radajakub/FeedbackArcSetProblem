#ifndef __ALNS_SHAKE_H__
#define __ALNS_SHAKE_H__

#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "graph.h"
#include "state.h"

const double NUM_INTERVALS_RATIO = 0.1;

namespace co {

    typedef std::function<void(State &s, DGraph &g, std::mt19937 &rng)> shaker;

    namespace shake {

        void swap_intervals(State &s, DGraph &g, std::mt19937 &rng);

    };
};

#endif
