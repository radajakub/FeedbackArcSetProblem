#include "alns_operators.h"

/// ########## DESTROY OPERATORS ##########

co::op_change co::destroy::random(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    s.remove_vertex(v, g);

    return {v};
}

co::op_change co::destroy::random_multiple(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> k_dist(2, (int)(MAX_DESTROY_RATIO * g.V));
    int k = k_dist(rng);

    std::vector<int> perm(g.V);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    std::vector<int> destroyed(k);
    for (int i = 0; i < k; ++i) {
        destroyed[i] = perm[i];
        s.remove_vertex(perm[i], g);
    }

    return destroyed;
}

co::op_change co::destroy::most_costly(DGraph &g, State &s, std::mt19937 &rng) {
    int max_cost = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int cost = 0;

        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[u.vertex] != -1 && s.ordering[v] > s.ordering[u.vertex]) {
                cost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[v]) {
            if (s.ordering[u.vertex] != -1 && s.ordering[v] < s.ordering[u.vertex]) {
                cost += u.cost;
            }
        }

        if (cost > max_cost) {
            max_cost = cost;
            max_vertex = v;
        }
    }

    s.remove_vertex(max_vertex, g);

    return {max_vertex};
}

co::op_change co::destroy::most_costly_multiple(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> k_dist(2, (int)(MAX_DESTROY_RATIO * g.V));
    int k = k_dist(rng);

    std::vector<int> costs(g.V, 0);

    // get costs of every vertex
    for (co::Edge &e : g.edges) {
        if (s.ordering[e.source] > s.ordering[e.target]) {
            costs[e.source] += e.cost;
        }
    }

    // argsort them
    std::vector<std::pair<int, int>> indexed_costs = co::enumerate(costs);
    std::sort(indexed_costs.begin(), indexed_costs.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first > b.first;
    });

    std::vector<int> destroyed(k);
    for (int i = 0; i < k; ++i) {
        destroyed[i] = indexed_costs[i].second;
        s.remove_vertex(indexed_costs[i].second, g);
    }

    return destroyed;
}

co::op_change co::destroy::high_degree(DGraph &g, State &s, std::mt19937 &rng) {
    int max_degree = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int degree = g.out_degrees[v] + g.in_degrees[v];
        if (degree > max_degree) {
            max_degree = degree;
            max_vertex = v;
        }
    }

    s.remove_vertex(max_vertex, g);

    return {max_vertex};
}

co::op_change co::destroy::mostly_backwards(DGraph &g, State &s, std::mt19937 &rng) {
    // compute backward cost and forward cost
    std::vector<int> forward_cost(g.V, 0);
    std::vector<int> backward_cost(g.V, 0);

    for (int v = 0; v < g.V; ++v) {
        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                forward_cost[v] += u.cost;
            } else {
                backward_cost[v] += u.cost;
            }
        }
    }

    // remove all which have higher cost backward than forward
    std::vector<int> destroyed;
    for (int i = 0; i < g.V; ++i) {
        if (backward_cost[i] > forward_cost[i]) {
            s.remove_vertex(i, g);
            destroyed.push_back(i);
        }
    }
    return destroyed;
}

co::op_change co::destroy::more_incoming(DGraph &g, State &s, std::mt19937 &rng) {
    // compute backward cost and forward cost
    std::vector<int> income_cost(g.V, 0);
    std::vector<int> outgoing_gain(g.V, 0);

    for (int v = 0; v < g.V; ++v) {
        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                outgoing_gain[v] += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                income_cost[v] += u.cost;
            }
        }
    }

    // remove all which have higher cost backward than forward
    std::vector<int> destroyed;
    for (int i = 0; i < g.V; ++i) {
        if (income_cost[i] > outgoing_gain[i]) {
            s.remove_vertex(i, g);
            destroyed.push_back(i);
        }
    }
    return destroyed;
}

/// ########## REPAIR OPERATORS ##########

void co::repair::random(DGraph &g, State &s, co::op_change destroyed, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);

    for (int v : destroyed) {
        int pos = v_dist(rng);
        s.place_vertex(v, pos, g);
    }
}

void co::repair::greedy(DGraph &g, State &s, co::op_change destroyed, std::mt19937 &rng) {
    // shuffle the destroyed vertices to provide randomness
    if (destroyed.size() > 1)
        std::shuffle(destroyed.begin(), destroyed.end(), rng);

    // for every destroyed vertex find the best position
    for (int v : destroyed) {
        int best_pos = -1;
        int best_cost = std::numeric_limits<int>::max();
        for (int pos = 0; pos < g.V; ++pos) {
            int cost = s.cost;
            for (co::Vertex &u : g.out_edges[v]) {
                if (s.ordering[u.vertex] != -1 && pos > s.ordering[u.vertex]) {
                    cost += u.cost;
                }
            }

            for (co::Vertex &u : g.in_edges[v]) {
                if (s.ordering[u.vertex] != -1 && pos <= s.ordering[u.vertex]) {
                    cost += u.cost;
                }
            }

            if (cost < best_cost) {
                best_pos = pos;
                best_cost = cost;
            }
        }
        s.place_vertex(v, best_pos, g);
    }
}
