#include "modifiers.h"

co::State co::modifiers::swap(co::DGraph &g, co::State &state, int u, int v) {
    co::State new_state = state;

    new_state.set_order(u, state.order[v]);
    new_state.set_order(v, state.order[u]);

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
    // compute costs of vertices
    std::vector<int> costs(g.V, 0);
    for (co::Edge &e : g.edges) {
        if (state.order[e.source] > state.order[e.target]) {
            costs[e.source] += e.cost;
            costs[e.target] += e.cost;
        }
    }

    int worst_cost = -1;
    int worst_v = -1;
    for (int v = 0; v < g.V; ++v) {
        if (costs[v] > worst_cost) {
            worst_cost = costs[v];
            worst_v = v;
        }
    }

    co::State improved = state;
    co::State new_state(g.V);
    for (int v = 0; v < g.V; ++v) {
        new_state = co::modifiers::swap(g, state, v, worst_v);
        if (new_state.value < improved.value) {
            improved = new_state;
        }
    }

    return improved;
}
