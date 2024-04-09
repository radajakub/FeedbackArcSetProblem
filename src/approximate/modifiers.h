#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <algorithm>
#include <vector>

#include "state.h"
#include "util.h"

namespace co {

    namespace modifiers {

        co::State swap(co::DGraph &g, co::State &state, int u, int v);
        co::State perturbate(co::DGraph &g, co::State &state, co::Sampler &sampler);

    };

};

#endif
