#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <algorithm>
#include <vector>

#include "state.h"
#include "util.h"

namespace co {

    namespace modifiers {
        // swap vertices u and v in the order
        State swap(DGraph &g, State &state, int u, int v);

        // swap two vertices randomly
        State perturbate(DGraph &g, State &state, Sampler &sampler);

        // find the vertex with the highest cost and find best position for it
        State improve_worst(DGraph &g, State &state);

        State swap_two_worst(DGraph &g, State &state);

        State best_rotation(DGraph &g, State &state);
    };

};

#endif
