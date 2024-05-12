#include "hill_climber.h"

co::State co::hill_climber(co::DGraph &g, co::Timer &timer, co::Sampler &sampler) {
    int best_cost = std::numeric_limits<int>::max();
    co::State best(g.V);

    int iters = 0;
    co::State current(g.V);

    while (!timer.should_stop()) {
        current = co::builders::random(g, sampler);
        if (iters == 0) {
            current = co::builders::in_cost(g);
        } else if (iters == 1) {
            current = co::builders::out_cost(g);
        } else if (iters == 2) {
            current = co::builders::in_degree(g);
        } else if (iters == 3) {
            current = co::builders::out_degree(g);
        }
        current.evaluate_full(g);

        // improve it while possible
        while (!timer.should_stop()) {
            co::State improved = co::modifiers::improve_worst(g, current);
            if (improved.ordering == current.ordering) {
                break;
            }

            current = improved;
        }

        if (current.cost < best_cost) {
            std::cout << "New best cost: " << current.cost << std::endl;
            best = current;
            best_cost = current.cost;
        }

        ++iters;
    }

    return best;
}
