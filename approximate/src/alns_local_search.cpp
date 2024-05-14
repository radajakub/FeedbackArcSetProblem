#include "alns_local_search.h"

co::State co::ls::swap_neighbors(State &s, DGraph &g, std::mt19937 &rng) {
    co::State new_s = s;

    std::vector<int> vertices(g.V);
    for (int i = 0; i < g.V; ++i) {
        vertices[new_s.ordering[i]] = i;
    }

    for (int j = 0; j < g.V - 1; ++j) {
        // vertex and right neighbor
        int u = vertices[j];
        int v = vertices[j + 1];

        // compute the costs between these two
        int uv = g.cost_matrix[u][v];
        int vu = g.cost_matrix[v][u];

        bool swap = false;
        // there is only backward edge
        if (uv == -1 && vu != -1) {
            swap = true;
        } else if (uv != -1 && vu != -1) {
            if (vu > uv) {
                swap = true;
            }
        }

        if (swap) {
            int u_pos = new_s.ordering[u];
            int v_pos = new_s.ordering[v];

            new_s.remove_vertex(u, g);
            new_s.remove_vertex(v, g);
            new_s.place_vertex(v, u_pos, g);
            new_s.place_vertex(u, v_pos, g);

            for (int i = 0; i < g.V; ++i) {
                vertices[new_s.ordering[i]] = i;
            }
        }
    }

    return new_s;
}

co::State co::ls::shift_range(State &s, DGraph &g, std::mt19937 &rng) {
    co::State new_s = s;

    std::vector<int> vertices(g.V);
    for (int i = 0; i < g.V; ++i) {
        vertices[new_s.ordering[i]] = i;
    }

    int start = std::uniform_int_distribution<int>(0, g.V - 3)(rng);
    int end = std::uniform_int_distribution<int>(start + 1, g.V - 1)(rng);

    std::vector<int> range(vertices.begin() + start, vertices.begin() + end);
    vertices.erase(vertices.begin() + start, vertices.begin() + end);

    int best_cost = std::numeric_limits<int>::max();
    co::State best_s = s;

    for (int i = 0; i < vertices.size(); ++i) {
        std::vector<int> new_vertices = vertices;
        new_vertices.insert(new_vertices.begin() + i, range.begin(), range.end());

        // convert vertex positions to ordering
        co::State new_s(g.V);
        for (int i = 0; i < g.V; ++i) {
            new_s.ordering[new_vertices[i]] = i;
        }
        new_s.evaluate_full(g);

        if (new_s.cost < best_cost) {
            best_cost = new_s.cost;
            best_s = new_s;
        }
    }

    return best_s;
}
