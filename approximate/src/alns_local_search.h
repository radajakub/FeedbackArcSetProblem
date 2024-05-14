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
        // DOES NOT WORK
        co::State swap_neighbors(State &s, DGraph &g, std::mt19937 &rng);

        // DOES NOT WORK
        co::State shift_range(State &s, DGraph &g, std::mt19937 &rng);
    };
};

#endif
