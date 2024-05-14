#include "utils.h"

std::vector<std::pair<int, int>> co::enumerate(std::vector<int> &vector) {
    std::vector<std::pair<int, int>> indexed(vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        indexed[i] = std::make_pair(vector[i], i);
    }
    return indexed;
}
