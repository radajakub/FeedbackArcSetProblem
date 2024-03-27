#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <fstream>
#include <vector>

#include "graph.h"

namespace co {
    class Solution {
    public:
        // sum of costs of removed edges
        int value;

        // removed edges from the Graph
        std::vector<co::Edge> D;

        Solution(std::vector<co::Edge> &edges);

        void save(std::string path);
    };
};

#endif
