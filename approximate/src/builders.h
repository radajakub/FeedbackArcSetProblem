#ifndef __BUILDERS_H__
#define __BUILDERS_H__

#include "graph.h"
#include "state.h"
#include "utils.h"

namespace co {

    namespace builders {

        State random(DGraph &g, std::mt19937 &rng);

        State in_degree(DGraph &g);
        State out_degree(DGraph &g);

        State in_cost(DGraph &g);
        State out_cost(DGraph &g);

    };

};

#endif
