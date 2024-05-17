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

    std::cout << "==== ls " << std::endl;

    std::uniform_int_distribution<int> dist(0, choices.size() - 1);
    return *std::next(choices.begin(), dist(rng));
}

void co::ls::LSSelector::update(int op, bool improvement) {
    if (!improvement) {
        // remove operator from choices
        choices.erase(op);
    }
}

void co::ls::LSSelector::reset() {
    std::cout << "---- reset ----" << std::endl;
    choices = std::unordered_set<int>(all.begin(), all.end());
}

std::pair<co::State, bool> co::ls::shift_range(State &s, DGraph &g, std::mt19937 &rng) {
    if (g.V < 5) {
        return std::make_pair(s, false);
    }

    co::State new_s = s;

    std::vector<int> vertices(g.V);
    for (int i = 0; i < g.V; ++i) {
        vertices[new_s.positions[i]] = i;
    }

    int start = std::uniform_int_distribution<int>(0, g.V - 3)(rng);
    int end = std::uniform_int_distribution<int>(start + 1, g.V - 1)(rng);

    std::vector<int> range(vertices.begin() + start, vertices.begin() + end);
    vertices.erase(vertices.begin() + start, vertices.begin() + end);

    int best_cost = std::numeric_limits<int>::max();
    co::State best_s = s;
    bool improved = false;

    for (int i = 0; i < vertices.size(); ++i) {
        std::vector<int> new_vertices = vertices;
        new_vertices.insert(new_vertices.begin() + i, range.begin(), range.end());

        // convert vertex positions to ordering
        co::State new_s(g.V);
        for (int i = 0; i < g.V; ++i) {
            new_s.positions[new_vertices[i]] = i;
        }
        new_s.evaluate_full(g);

        if (new_s.cost < best_cost) {
            best_cost = new_s.cost;
            best_s = new_s;
            improved = true;
        }
    }

    return std::make_pair(best_s, improved);
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
