#include "alns_builders.h"

co::State co::build::random(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::iota(s.positions.begin(), s.positions.end(), 0);
    std::shuffle(s.positions.begin(), s.positions.end(), rng);

    return s;
}

co::State co::build::in_degree(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_indegrees = g.in_degree_vertices();
    s.set_order(indexed_indegrees);

    return s;
}

co::State co::build::out_degree(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outdegrees = g.out_degree_vertices();
    s.set_order(indexed_outdegrees);

    return s;
}

co::State co::build::in_cost(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_incosts = g.in_cost_vertices();
    s.set_order(indexed_incosts);

    return s;
}

co::State co::build::out_cost(co::DGraph &g, std::mt19937 &rng) {
    co::State s(g.V);

    std::vector<std::pair<int, int>> indexed_outcosts = g.out_cost_vertices();
    s.set_order(indexed_outcosts);

    return s;
}

co::State co::build::_bidirect(co::DGraph &g, std::vector<int> &order) {
    co::State s(g.V);
    int l = 0;
    int u = g.V - 1;
    for (int v : order) {
        if (g.in_costs[v] < g.out_costs[v]) {
            s.positions[v] = l++;
        } else {
            s.positions[v] = u--;
        }
    }

    return s;
}

co::State co::build::bidirect(co::DGraph &g, std::mt19937 &rng) {
    // positions is 0, 1, 2, ..., V - 1
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);

    return co::build::_bidirect(g, order);
}

co::State co::build::bidirect_shuffle(co::DGraph &g, std::mt19937 &rng) {
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), rng);

    return co::build::_bidirect(g, order);
}

co::State co::build::bidirect_max(co::DGraph &g, std::mt19937 &rng) {
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(), [&g](int a, int b) {
        int wa = std::max(g.in_costs[a], g.out_costs[a]);
        int wb = std::max(g.in_costs[b], g.out_costs[b]);
        return wa > wb;
    });

    return co::build::_bidirect(g, order);
}

co::State co::build::bidirect_min(co::DGraph &g, std::mt19937 &rng) {
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(), [&g](int a, int b) {
        int wa = std::min(g.in_costs[a], g.out_costs[a]);
        int wb = std::min(g.in_costs[b], g.out_costs[b]);
        return wa < wb;
    });

    return co::build::_bidirect(g, order);
}

co::State co::build::bidirect_abs(co::DGraph &g, std::mt19937 &rng) {
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(), [&g](int a, int b) {
        int wa = std::abs(g.in_costs[a] - g.out_costs[a]);
        int wb = std::abs(g.in_costs[b] - g.out_costs[b]);
        return wa > wb;
    });

    return co::build::_bidirect(g, order);
}

co::State co::build::bidirect_ratio(co::DGraph &g, std::mt19937 &rng) {
    std::vector<int> order(g.V);
    std::iota(order.begin(), order.end(), 0);

    std::vector<double> rs(g.V);
    for (int i = 0; i < g.V; ++i) {
        double r1 = (double)g.in_costs[i] / g.out_costs[i];
        double r2 = (double)g.out_costs[i] / g.in_costs[i];
        rs[i] = std::max(r1, r2);
    }

    std::sort(order.begin(), order.end(), [&g, &rs](int a, int b) {
        return rs[a] > rs[b];
    });

    return co::build::_bidirect(g, order);
}
