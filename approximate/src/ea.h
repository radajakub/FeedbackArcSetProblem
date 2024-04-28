#ifndef __EA_H__
#define __EA_H__

#include <iostream>
#include <numeric>
#include <vector>

#include "builders.h"
#include "graph.h"
#include "state.h"
#include "utils.h"

namespace co {
    namespace ea {

        class EvolutionConfiguration {
        public:
            int population_size;
            int select_size;
            int tournament_size;
            double mutation_probability;

            EvolutionConfiguration(int population_size, int select_size, int tournament_size, double mutation_probability) : population_size(population_size), select_size(select_size), tournament_size(tournament_size), mutation_probability(mutation_probability){};
        };

        State find_bsf(std::vector<State> &population);

        std::vector<State> init(DGraph &g, int n, Sampler &sampler);

        // select k states (indices) from population by running tournament selection
        std::vector<int> select(std::vector<State> &population, int k, int tournamen_size, Sampler &sampler);

        State cross(DGraph &g, State &parent1, State &parent2, Sampler &sampler);

        State mutate(DGraph &g, State &state, Sampler &sampler);

        State evolve(DGraph &g, EvolutionConfiguration &config, Sampler &sampler, Timer &timer);
    };
};

#endif
