#ifndef __EA_H__
#define __EA_H__

#include <iostream>
#include <numeric>
#include <vector>

#include "builders.h"
#include "graph.h"
#include "modifiers.h"
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

        // find the best state in the vector of states
        State find_bsf(std::vector<State> &population);

        // initialize population of size n
        std::vector<State> init(DGraph &g, int n, Sampler &sampler);

        // select k states (indices) from population by running tournament selection
        std::vector<int> select(std::vector<State> &population, int k, int tournamen_size, Sampler &sampler);

        std::pair<co::State, co::State> pmx_cross(DGraph &g, State &p1, State &p2, Sampler &sampler);

        State inversion_cross(DGraph &g, State &p1, State &p2, Sampler &sampler);

        // State ox1_cross(DGraph &g, State &p1, State &p2, Sampler &sampler);

        State mutate(DGraph &g, State &state, Sampler &sampler);

        State evolve(DGraph &g, EvolutionConfiguration &config, Sampler &sampler, Timer &timer);
    };
};

#endif
