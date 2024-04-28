#include "ea.h"

co::State co::ea::find_bsf(std::vector<co::State> &population) {
    int best_index = -1;
    int best_value = std::numeric_limits<int>::max();
    for (int i = 0; i < population.size(); ++i) {
        co::State &s = population[i];
        if (s.cost < best_value) {
            best_index = i;
            best_value = s.cost;
        }
    }
    return population[best_index];
}

std::vector<co::State> co::ea::init(DGraph &g, int n, Sampler &sampler) {
    std::vector<co::State> population;
    population.reserve(n);

    population.push_back(co::builders::out_degree(g));
    population.push_back(co::builders::in_degree(g));
    population.push_back(co::builders::out_cost(g));
    population.push_back(co::builders::in_cost(g));
    for (int i = 4; i < n; ++i) {
        population.push_back(co::builders::random(g, sampler));
    }

    for (co::State &s : population) {
        s.evaluate_full(g);
        std::cout << s.cost << std::endl;
    }

    return population;
}

co::State co::ea::evolve(DGraph &g, EvolutionConfiguration &config, Sampler &sampler, Timer &timer) {
    // initalize
    std::vector<co::State> population = co::ea::init(g, config.population_size, sampler);
    co::State bsf = co::ea::find_bsf(population);

    // evolve while there is time
    // while (!timer.should_stop()) {
    // }

    return bsf;
}
