#include "solvers.h"

co::State co::hc::random_greedy(co::DGraph &g, co::Sampler &sampler, co::Timer &timer) {
    co::State state = co::builders::random(g, sampler);
    state.evaluate_full(g);

    co::State best = state;
    while (!timer.should_stop()) {
        state = co::modifiers::perturbate(g, state, sampler);

        if (state.cost < best.cost) {
            best = state;
        }
    }

    return best;
}

co::State co::hc::greedy_improve(co::DGraph &g, co::Sampler &sampler, co::Timer &timer) {
    co::State state = co::builders::in_degree(g);
    state.evaluate_full(g);

    co::State best = state;
    while (!timer.should_stop()) {
        double p = sampler.sample_prob();
        if (p < 0.5) {
            state = co::modifiers::perturbate(g, state, sampler);
        }
        state = co::modifiers::improve_worst(g, state);

        if (state.cost < best.cost) {
            best = state;
        }
    }

    return best;
}
