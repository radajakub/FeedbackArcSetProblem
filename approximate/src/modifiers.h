#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <algorithm>
#include <vector>

#include "state.h"
#include "utils.h"

namespace co {

    namespace modifiers {
        // swap vertices u and v in the order
        State swap(DGraph &g, State &state, int u, int v);

        // swap two vertices randomly
        State perturbate(DGraph &g, State &state, Sampler &sampler);

        // find the vertex with the highest cost and find best position for it
        State improve_worst(DGraph &g, State &state);

        State ls(DGraph &g, State &state, co::Sampler &sampler);
    };

};

#endif
