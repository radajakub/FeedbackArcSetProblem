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

int co::ls::LSSelector::select_all() {
    std::uniform_int_distribution<int> dist(0, this->all.size() - 1);
    int op = dist(rng);
    return this->all[op];
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

std::pair<co::State, bool> co::ls::all_k_swaps(co::State &s, co::DGraph &g, std::mt19937 &rng) {
    co::State best = s;

    int k_max = g.V;
    int k = std::min(k_max, g.V - 2);

    bool improved = false;

#pragma omp parallel for
    for (int u = 0; u < g.V - k; ++u) {
        for (int i = 1; i <= k; ++i) {
            std::vector<int> vertices = s.to_vertices();
            std::swap(vertices[u], vertices[u + i]);

            co::State new_s(g.V);
            new_s.from_vertices(vertices);
            new_s.evaluate_full(g);

#pragma omp critical
            if (new_s.cost < best.cost) {
                best = new_s;
                improved = true;
            }
        }
    }

    return std::make_pair(best, improved);
}

std::pair<co::State, bool> co::ls::shift_range(co::State &s, co::DGraph &g, std::mt19937 &rng) {
    co::State best = s;

    int length = 5;
    int trials = 10;

    bool improved = false;

#pragma omp parallel for
    for (int u = 0; u < g.V - length; ++u) {
        std::vector<int> vertices = s.to_vertices();
        auto start = vertices.begin() + u;
        auto end = start + length;
        std::vector<int> range(start, end);
        vertices.erase(start, end);

        std::vector<int> perm = co::randperm(g.V - length, rng);

        for (int i = 0; i < trials; ++i) {
            int v = perm[i];
            std::vector<int> new_vertices = vertices;
            new_vertices.insert(new_vertices.begin() + v, range.begin(), range.end());

            co::State new_s(g.V);
            new_s.from_vertices(new_vertices);
            new_s.evaluate_full(g);

#pragma omp critical
            if (new_s.cost < best.cost) {
                best = new_s;
                improved = true;
            }
        }
    }

    return std::make_pair(best, improved);
}

std::pair<co::State, bool> co::ls::range_reversal(co::State &s, co::DGraph &g, std::mt19937 &rng) {
    co::State best = s;

    int length = 5;

    bool improved = false;

#pragma omp parallel for
    for (int i = 0; i < g.V - length; ++i) {
        std::vector<int> vertices = s.to_vertices();
        std::reverse(vertices.begin() + i, vertices.begin() + i + length);

        co::State new_s(g.V);
        new_s.from_vertices(vertices);
        new_s.evaluate_full(g);

#pragma omp critical
        if (new_s.cost < best.cost) {
            best = new_s;
            improved = true;
        }
    }

    return std::make_pair(best, improved);
}
