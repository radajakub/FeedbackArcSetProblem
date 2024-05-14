#include "alns_local_search.h"

void co::ls::best_swap(co::State &s, co::DGraph &g, std::mt19937 &rng) {
    // std::cout << "best swap" << std::endl;
    // create random order

    std::uniform_int_distribution<int> dist(0, g.V - 1);
    int v = dist(rng);

    // find best swap
    int best_cost = s.cost;
    int best_swap = s.ordering[v];

    for (int u = 0; u < g.V; ++u) {
        if (u == v) continue;

        co::State s_copy = s;
        int v_pos = s_copy.ordering[v];
        int u_pos = s_copy.ordering[u];

        s_copy.remove_vertex(v, g);
        s_copy.remove_vertex(u, g);
        s_copy.place_vertex(v, u_pos, g);
        s_copy.place_vertex(u, v_pos, g);
        if (s_copy.cost < best_cost) {
            best_cost = s_copy.cost;
            best_swap = u;
        }
    }

    if (best_swap != s.ordering[v]) {
        int v_pos = s.ordering[v];
        int u_pos = s.ordering[best_swap];

        s.remove_vertex(v, g);
        s.remove_vertex(best_swap, g);
        s.place_vertex(v, u_pos, g);
        s.place_vertex(best_swap, v_pos, g);
    }
}
