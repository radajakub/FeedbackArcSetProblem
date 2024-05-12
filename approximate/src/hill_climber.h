#ifndef __HILL_CLIMBER_H__
#define __HILL_CLIMBER_H__

#include <limits>
#include <unordered_set>

#include "builders.h"
#include "graph.h"
#include "modifiers.h"
#include "state.h"
#include "utils.h"

namespace co {

    co::State hill_climber(co::DGraph &g, co::Timer &timer, co::Sampler &sampler);

};

#endif
