#ifndef __EXACT_H__
#define __EXACT_H__

#include <limits>
#include <numeric>
#include <stack>
#include <vector>

#include "graph.h"
#include "state.h"

namespace co {

    State branch_and_bound(DGraph &g);

};

#endif
