#ifndef __SOLVERS_H__
#define __SOLVERS_H__

#include "builders.h"
#include "modifiers.h"
#include "state.h"
#include "util.h"

namespace co {
    namespace hc {

        State random_greedy(DGraph &g, Sampler &sampler, Timer &timer);

        State greedy_improve(DGraph &g, Sampler &sampler, Timer &timer);

    };
};

#endif
