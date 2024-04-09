#include "solvers.h"

co::State co::solvers::random_greedy(co::DGraph &g, co::Sampler &sampler, co::Timer &timer) {
    co::State state = co::builders::random(g, sampler);
    state.evaluate_full(g);

    std::cout << "here" << std::endl;

    co::State best = state;
    while (!timer.should_stop()) {
        state = co::modifiers::perturbate(g, state, sampler);

        if (state.value < best.value) {
            best = state;
        }
    }

    return best;
}

co::State co::solvers::greedy_improve(co::DGraph &g, co::Sampler &sampler, co::Timer &timer) {
    co::State state = co::builders::in_degree(g);
    state.evaluate_full(g);

    co::State best = state;
    while (!timer.should_stop()) {
        if (sampler.sample_prob() < 0.5) {
            state = co::modifiers::perturbate(g, state, sampler);
        }
        state = co::modifiers::improve_worst(g, state);

        if (state.value < best.value) {
            best = state;
        }
    }

    return best;
}
