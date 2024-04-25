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
