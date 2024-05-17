#include "alns_local_search.h"

co::ls::LSSelector::LSSelector(int operators, std::mt19937 &rng) : rng(rng) {
    all.resize(operators);
    std::iota(all.begin(), all.end(), 0);
    for (int i = 0; i < operators; ++i) {
        choices.insert(i);
    }
}

int co::ls::LSSelector::select() {
    if (choices.empty()) {
        return -1;
    }

    std::uniform_int_distribution<int> dist(0, choices.size() - 1);
    return *std::next(choices.begin(), dist(rng));
}

void co::ls::LSSelector::update(int op, bool improvement) {
    if (!improvement) {
        choices.erase(op);
    }
}

void co::ls::LSSelector::reset() {
    choices = std::unordered_set<int>(all.begin(), all.end());
}

std::pair<co::State, bool> co::ls::all_two_ops(State &s, DGraph &g, std::mt19937 &rng) {
    co::State best = s;

    int length = 5;

    bool improved = false;

#pragma omp parallel for
    for (int l = 2; l <= length; ++l) {
        for (int u = 0; u < g.V - l; ++u) {
            std::vector<int> vertices = s.to_vertices();
            std::reverse(vertices.begin() + u, vertices.begin() + u + l);

            co::State new_s(g.V);
            new_s.from_vertices(vertices);
            new_s.evaluate_full(g);

#pragma omp critical
            if (new_s.cost < best.cost) {
                improved = true;
                best = new_s;
            }
        }
    }

    return std::make_pair(best, improved);
}
