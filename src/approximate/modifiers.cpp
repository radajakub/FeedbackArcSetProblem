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
