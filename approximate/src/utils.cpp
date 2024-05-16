#include "utils.h"

std::vector<std::pair<int, int>> co::enumerate(std::vector<int> &vector) {
    std::vector<std::pair<int, int>> indexed(vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        indexed[i] = std::make_pair(vector[i], i);
    }
    return indexed;
}

std::vector<int> co::randperm(int n, std::mt19937 &rng) {
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);
    return perm;
}

int co::argmax(std::vector<double> &vector) {
    int idx = 0;
    double max_val = 0;
    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i] > max_val) {
            max_val = vector[i];
            idx = i;
        }
    }

    return idx;
}

int co::argmax(std::vector<int> &vector) {
    int idx = 0;
    int max_val = 0;
    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i] > max_val) {
            max_val = vector[i];
            idx = i;
        }
    }

    return idx;
}

void co::check_duplicates(std::vector<int> &vector) {
    std::unordered_set<int> visited;
    for (int i = 0; i < vector.size(); ++i) {
        if (visited.find(vector[i]) != visited.end()) {
            std::cout << "Duplicate element in vector" << std::endl;
            exit(1);
        }
        visited.insert(vector[i]);
    }
}
