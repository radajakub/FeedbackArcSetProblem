#ifndef __ALNS_LOCAL_SEARCH_H__
#define __ALNS_LOCAL_SEARCH_H__

#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph.h"
#include "state.h"
#include "utils.h"

namespace co {

    typedef std::function<std::pair<State, bool>(State &s, DGraph &g, std::mt19937 &rng)> ls_op;

    namespace ls {
        class LSSelector {
        private:
            std::mt19937 rng;

            std::vector<int> all;
            std::unordered_set<int> choices;

        public:
            LSSelector(int operators, std::mt19937 &rng);

            int select();

            int select_all();

            void update(int op, bool improvement);
            void reset();
        };

        std::pair<State, bool> all_two_ops(State &s, DGraph &g, std::mt19937 &rng);

        std::pair<State, bool> all_k_swaps(State &s, DGraph &g, std::mt19937 &rng);

        std::pair<State, bool> shift_range(State &s, DGraph &g, std::mt19937 &rng);

        std::pair<State, bool> range_reversal(State &s, DGraph &g, std::mt19937 &rng);
    };
};

#endif
