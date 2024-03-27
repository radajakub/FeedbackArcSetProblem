#ifndef __MILP_H__
#define __MILP_H__

#include <gurobi_c++.h>

#include "graph.h"
#include "solution.h"

namespace co {
    co::Solution solve_milp(co::DGraph &g);
};

#endif
