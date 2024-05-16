#ifndef __UTILS_H__
#define __UTILS_H__

#include <algorithm>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

namespace co {
    // wrap every item with an index as the second item of the tuple
    std::vector<std::pair<int, int>> enumerate(std::vector<int> &vector);

    std::vector<int> randperm(int n, std::mt19937 &rng);

    int argmax(std::vector<double> &vector);
    int argmax(std::vector<int> &vector);
};

#endif
