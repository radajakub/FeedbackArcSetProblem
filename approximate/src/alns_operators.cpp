#include "alns_operators.h"

/// ########## DESTROY OPERATORS ##########

co::op_change co::destroy::random(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    s.remove_vertex(v, g);

    return {v};
}

co::op_change co::destroy::random_multiple(DGraph &g, State &s, std::mt19937 &rng) {
    int k_min = 2;
    int k_max = std::max((int)(MAX_DESTROY_RATIO * g.V), k_min + 1);

    std::uniform_int_distribution<int> k_dist(k_min, k_max);
    int k = k_dist(rng);

    std::vector<int> perm = co::randperm(g.V, rng);

    std::vector<int> destroyed(k);
    for (int i = 0; i < k; ++i) {
        int v = perm[i];
        s.remove_vertex(v, g);
        destroyed[i] = v;
    }

    return destroyed;
}

co::op_change co::destroy::random_range(DGraph &g, State &s, std::mt19937 &rng) {
    int k_min = 2;
    int k_max = std::max((int)(MAX_DESTROY_RATIO * g.V), k_min + 1);

    std::uniform_int_distribution<int> k_dist(k_min, k_max);
    int k = k_dist(rng);
    std::uniform_int_distribution<int> start_dist(0, g.V - k - 1);
    int start = start_dist(rng);

    std::vector<int> vertices = s.to_vertices();

    std::vector<int> destroyed(vertices.begin() + start, vertices.begin() + start + k);
    for (int d : destroyed) {
        s.remove_vertex(d, g);
    }

    return destroyed;
}

co::op_change co::destroy::backward_adjacent(DGraph &g, State &s, std::mt19937 &rng) {
    // sample random vertex
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    std::vector<int> destroyed;

    // find all edges that go backwards and find the vertices
    for (co::Vertex &u : g.out_edges[v]) {
        if (s.positions[v] > s.positions[u.vertex]) {
            destroyed.push_back(u.vertex);
        }
    }

    for (co::Vertex &u : g.in_edges[v]) {
        if (s.positions[v] < s.positions[u.vertex]) {
            destroyed.push_back(u.vertex);
        }
    }

    for (int d : destroyed) {
        s.remove_vertex(d, g);
    }

    return destroyed;
}

co::op_change co::destroy::adjacent(DGraph &g, State &s, std::mt19937 &rng) {
    // sample random vertex
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    std::vector<int> pre_destroyed;
    pre_destroyed.reserve(g.out_edges[v].size() + g.in_edges[v].size());

    // find all edges that go backwards and find the vertices
    for (co::Vertex &u : g.out_edges[v]) {
        pre_destroyed.push_back(u.vertex);
    }

    for (co::Vertex &u : g.in_edges[v]) {
        pre_destroyed.push_back(u.vertex);
    }

    std::vector<int> destroyed;
    for (int d : pre_destroyed) {
        if (s.positions[d] != -1) {
            s.remove_vertex(d, g);
            destroyed.push_back(d);
        }
    }

    return destroyed;
}

co::op_change co::destroy::most_costly(DGraph &g, State &s, std::mt19937 &rng) {
    int max_cost = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int cost = 0;

        for (co::Vertex &u : g.out_edges[v]) {
            if (s.positions[u.vertex] != -1 && s.positions[v] > s.positions[u.vertex]) {
                cost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[v]) {
            if (s.positions[u.vertex] != -1 && s.positions[v] < s.positions[u.vertex]) {
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
    int k_min = 2;
    int k_max = std::max((int)(MAX_DESTROY_RATIO * g.V), k_min + 1);

    std::uniform_int_distribution<int> k_dist(k_min, k_max);
    int k = k_dist(rng);

    std::vector<int> costs(g.V, 0);

    // get costs of every vertex
    for (co::Edge &e : g.edges) {
        if (s.positions[e.source] > s.positions[e.target]) {
            costs[e.source] += e.cost;
        }
    }

    // argsort them
    std::vector<std::pair<int, int>> indexed_costs = co::enumerate(costs);
    std::sort(indexed_costs.begin(), indexed_costs.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first > b.first;
    });

    std::vector<int> destroyed(k, -1);
    for (int i = 0; i < k; ++i) {
        int v = indexed_costs[i].second;
        destroyed[i] = v;
        s.remove_vertex(v, g);
    }

    return destroyed;
}

co::op_change co::destroy::most_costly_adjacent(DGraph &g, State &s, std::mt19937 &rng) {
    int max_cost = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int cost = 0;

        for (co::Vertex &u : g.out_edges[v]) {
            if (s.positions[u.vertex] != -1 && s.positions[v] > s.positions[u.vertex]) {
                cost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[v]) {
            if (s.positions[u.vertex] != -1 && s.positions[v] < s.positions[u.vertex]) {
                cost += u.cost;
            }
        }

        if (cost > max_cost) {
            max_cost = cost;
            max_vertex = v;
        }
    }

    std::vector<int> destroyed;

    // find all edges that go backwards and find the vertices
    for (co::Vertex &u : g.out_edges[max_vertex]) {
        if (s.positions[max_vertex] > s.positions[u.vertex]) {
            destroyed.push_back(u.vertex);
        }
    }

    for (co::Vertex &u : g.in_edges[max_vertex]) {
        if (s.positions[max_vertex] < s.positions[u.vertex]) {
            destroyed.push_back(u.vertex);
        }
    }

    for (int d : destroyed) {
        // check in case the vertex was already removed
        if (s.positions[d] != -1) {
            s.remove_vertex(d, g);
        }
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
            if (s.positions[v] < s.positions[u.vertex]) {
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
            if (s.positions[v] < s.positions[u.vertex]) {
                outgoing_gain[v] += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (s.positions[v] < s.positions[u.vertex]) {
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
    std::shuffle(destroyed.begin(), destroyed.end(), rng);

    // for every destroyed vertex find the best position
    for (int v : destroyed) {
        int best_pos = -1;
        int best_cost = std::numeric_limits<int>::max();
        for (int pos = 0; pos < g.V; ++pos) {
            int cost = s.cost;
            for (co::Vertex &u : g.out_edges[v]) {
                if (s.positions[u.vertex] != -1 && pos > s.positions[u.vertex]) {
                    cost += u.cost;
                }
            }

            for (co::Vertex &u : g.in_edges[v]) {
                if (s.positions[u.vertex] != -1 && pos <= s.positions[u.vertex]) {
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
