#ifndef __UTILS_H__
#define __UTILS_H__

#include <utility>
#include <vector>

namespace co {
    // wrap every item with an index as the second item of the tuple
    std::vector<std::pair<int, int>> enumerate(std::vector<int> &vector);
};

#endif
