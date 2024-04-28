#include "builders.h"

co::State co::builders::random(co::DGraph &g, co::Sampler &sampler) {
    co::State s(g.V);

    std::iota(s.ordering.begin(), s.ordering.end(), 0);
    std::shuffle(s.ordering.begin(), s.ordering.end(), sampler.rng);

    return s;
}

co::State co::builders::in_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_indegrees = co::index_vector(g.in_degrees);

    // sort by in-degrees
    std::sort(indexed_indegrees.begin(), indexed_indegrees.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.out_degrees[a.second] > g.out_degrees[b.second];
        }
        return a.first < b.first;
    });

    s.set_order(indexed_indegrees);

    return s;
}

co::State co::builders::out_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outdegrees = co::index_vector(g.out_degrees);

    // sort by in-degrees
    std::sort(indexed_outdegrees.begin(), indexed_outdegrees.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.in_degrees[a.second] < g.in_degrees[b.second];
        }
        return a.first > b.first;
    });

    s.set_order(indexed_outdegrees);

    return s;
}

co::State co::builders::in_cost(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_incosts = co::index_vector(g.in_costs);

    // sort by in-costs from lowest to highest
    std::sort(indexed_incosts.begin(), indexed_incosts.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.out_costs[a.second] > g.out_costs[b.second];
        }
        return a.first < b.first;
    });

    s.set_order(indexed_incosts);

    return s;
}

co::State co::builders::out_cost(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outcosts = co::index_vector(g.out_costs);

    // sort by in-costs from lowest to highest
    std::sort(indexed_outcosts.begin(), indexed_outcosts.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.in_costs[a.second] < g.in_costs[b.second];
        }
        return a.first > b.first;
    });

    s.set_order(indexed_outcosts);

    return s;
}
