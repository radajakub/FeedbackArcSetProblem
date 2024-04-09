#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <algorithm>
#include <vector>

#include "state.h"
#include "util.h"

namespace co {

    namespace modifiers {

        State swap(DGraph &g, State &state, int u, int v);
        State perturbate(DGraph &g, State &state, Sampler &sampler);

        State improve_worst(DGraph &g, State &state);

    };

};

#endif
