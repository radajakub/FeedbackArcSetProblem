#include "modifiers.h"

co::State co::modifiers::swap(co::DGraph &g, co::State &state, int u, int v) {
    co::State new_state = state;

    new_state.set_order(u, state.ordering[v]);
    new_state.set_order(v, state.ordering[u]);

    std::vector<int> changed = {u, v};

    new_state.evaluate_incremental(g, state, changed);

    return new_state;
}

co::State co::modifiers::perturbate(co::DGraph &g, co::State &state, co::Sampler &sampler) {
    int u = sampler.sample_vertex();
    int v = sampler.sample_vertex();
    while (u == v) {
        v = sampler.sample_vertex();
    }

    return co::modifiers::swap(g, state, u, v);
}

co::State co::modifiers::improve_worst(co::DGraph &g, co::State &state) {
    // compute outcosts of vertices
    std::vector<int> costs(g.V, 0);
    for (co::Edge &e : g.edges) {
        if (state.ordering[e.source] > state.ordering[e.target]) {
            costs[e.source] += e.cost;
        }
    }

    // find the vertex with the highest cost
    int worst_v = co::argmax(costs);

    co::State improved = state;
    co::State new_state(g.V);
    for (int v = 0; v < g.V; ++v) {
        new_state = co::modifiers::swap(g, state, v, worst_v);
        if (new_state.cost < improved.cost) {
            improved = new_state;
        }
    }

    return improved;
}

co::State co::modifiers::swap_two_worst(co::DGraph &g, co::State &state) {
    // compute outcosts of vertices
    std::vector<int> costs(g.V, 0);
    for (co::Edge &e : g.edges) {
        if (state.ordering[e.source] > state.ordering[e.target]) {
            costs[e.source] += e.cost;
        }
    }

    // find the two vertices with the highest cost
    int worst_v = co::argmax(costs);
    costs[worst_v] = 0;
    int second_worst_v = co::argmax(costs);

    return co::modifiers::swap(g, state, worst_v, second_worst_v);
}

co::State co::modifiers::best_rotation(co::DGraph &g, co::State &state) {
    co::State best = state;

    co::State new_state = state;
    for (int i = 0; i < g.V; ++i) {
        std::vector<int> vertices = new_state.get_vertex_permutation();

        std::rotate(vertices.begin(), vertices.begin() + 1, vertices.end());

        new_state.update_ordering(vertices);
        new_state.evaluate_full(g);
        if (new_state.cost < best.cost) {
            best = new_state;
        }
    }

    return best;
}
