#include "solvers.h"

co::State co::solvers::random_greedy(co::DGraph &g, co::Sampler &sampler, co::Timer &timer) {
    co::State state = co::builders::random(g, sampler);
    state.evaluate_full(g);

    co::State best = state;
    while (!timer.should_stop()) {
        state = co::modifiers::perturbate(g, state, sampler);

        if (state.value < best.value) {
            best = state;
        }
    }

    return best;
}
