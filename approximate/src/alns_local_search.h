#ifndef __ALNS_LOCAL_SEARCH_H__
#define __ALNS_LOCAL_SEARCH_H__

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

#include "graph.h"
#include "state.h"

namespace co {
    namespace ls {
        // go through all vertices in random order and find the best swap with all others
        void best_swap(State &s, DGraph &g, std::mt19937 &rng);
    };
};

#endif
