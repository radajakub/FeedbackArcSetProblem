#ifndef __ALNS_BUILDERS_H__
#define __ALNS_BUILDERS_H__

#include <functional>

#include "graph.h"
#include "state.h"
#include "utils.h"

namespace co {

    typedef std::function<State(DGraph &g, std::mt19937 &rng)> builder;

    namespace build {

        State random(DGraph &g, std::mt19937 &rng);

        State in_degree(DGraph &g, std::mt19937 &rng);
        State out_degree(DGraph &g, std::mt19937 &rng);
        State in_cost(DGraph &g, std::mt19937 &rng);
        State out_cost(DGraph &g, std::mt19937 &rng);

        State _bidirect(DGraph &g, std::vector<int> &order);
        State bidirect(DGraph &g, std::mt19937 &rng);
        State bidirect_shuffle(DGraph &, std::mt19937 &rng);
        State bidirect_max(DGraph &g, std::mt19937 &rng);
        State bidirect_min(DGraph &g, std::mt19937 &rng);
        State bidirect_abs(DGraph &g, std::mt19937 &rng);
        State bidirect_ratio(DGraph &g, std::mt19937 &rng);
    };

};

#endif
