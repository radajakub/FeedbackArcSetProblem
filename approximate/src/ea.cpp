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

    // push all heuristics
    population.push_back(co::builders::out_degree(g));
    population.push_back(co::builders::in_degree(g));
    population.push_back(co::builders::out_cost(g));
    population.push_back(co::builders::in_cost(g));

    // fill with randomly sampled
    for (int i = 4; i < n; ++i) {
        population.push_back(co::builders::random(g, sampler));
    }

    for (co::State &s : population) {
        s.evaluate_full(g);
        std::cout << s.cost << std::endl;
    }

    return population;
}

std::vector<int> co::ea::select(std::vector<co::State> &population, int k, int tournamen_size, co::Sampler &sampler) {
    std::vector<int> indices(k);
    std::cout << "Select " << k << " candidates by tournaments" << std::endl;

    for (int i = 0; i < k; ++i) {
        int winner_idx = sampler.sample_population();
        for (int j = 1; j < tournamen_size; ++j) {
            int competitor_idx = sampler.sample_population();
            if (population[competitor_idx].cost < population[winner_idx].cost) {
                winner_idx = competitor_idx;
            }
        }
        indices[i] = winner_idx;
    }
    return indices;
}

co::State co::ea::evolve(DGraph &g, EvolutionConfiguration &config, co::Sampler &sampler, co::Timer &timer) {
    // initalize
    std::vector<co::State> population = co::ea::init(g, config.population_size, sampler);
    co::State bsf = co::ea::find_bsf(population);

    // evolve while there is time
    // while (!timer.should_stop()) {
    // select states for breeding
    std::vector<int> parents = co::ea::select(population, config.select_size, config.tournament_size, sampler);

    // breed all pairs

    // mutate randomly and add to new population

    // join previous and new population

    // }

    return bsf;
}
