#include "utils.h"

int co::argmax(std::vector<int> &vec) {
    int max_val = -1;
    int max_idx = -1;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > max_val) {
            max_val = vec[i];
            max_idx = i;
        }
    }
    return max_idx;
}

std::vector<std::pair<int, int>> co::enumerate(std::vector<int> &vector) {
    std::vector<std::pair<int, int>> indexed(vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        indexed[i] = std::make_pair(vector[i], i);
    }
    return indexed;
}
