#include "builders.h"

co::State co::builders::random(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::iota(s.ordering.begin(), s.ordering.end(), 0);
    std::shuffle(s.ordering.begin(), s.ordering.end(), rng);

    return s;
}

co::State co::builders::in_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_indegrees = g.in_degree_vertices();
    s.set_order(indexed_indegrees);

    return s;
}

co::State co::builders::out_degree(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outdegrees = g.out_degree_vertices();
    s.set_order(indexed_outdegrees);

    return s;
}

co::State co::builders::in_cost(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_incosts = g.in_cost_vertices();
    s.set_order(indexed_incosts);

    return s;
}

co::State co::builders::out_cost(co::DGraph &g) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outcosts = g.out_cost_vertices();
    s.set_order(indexed_outcosts);

    return s;
}
