#include "builders.h"

co::State co::builders::random(co::DGraph &g, co::Sampler &sampler) {
    co::State s(g.V);

    std::iota(s.order.begin(), s.order.end(), 0);
    std::shuffle(s.order.begin(), s.order.end(), sampler.rng);

    return s;
}

co::State co::builders::in_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_indegrees(g.V);
    for (int i = 0; i < g.V; ++i) {
        indexed_indegrees[i] = std::make_pair(g.in_degrees[i], i);
    }

    // sort by in-degrees
    std::sort(indexed_indegrees.begin(), indexed_indegrees.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.out_degrees[a.second] > g.out_degrees[b.second];
        }
        return a.first < b.first;
    });

    for (int i = 0; i < g.V; ++i) {
        s.set_order(indexed_indegrees[i].second, i);
    }

    return s;
}

co::State co::builders::out_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outdegrees(g.V);
    for (int i = 0; i < g.V; ++i) {
        indexed_outdegrees[i] = std::make_pair(g.out_degrees[i], i);
    }

    // sort by in-degrees
    std::sort(indexed_outdegrees.begin(), indexed_outdegrees.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.in_degrees[a.second] < g.in_degrees[b.second];
        }
        return a.first > b.first;
    });

    for (int i = 0; i < g.V; ++i) {
        s.set_order(indexed_outdegrees[i].second, i);
    }

    return s;
}
