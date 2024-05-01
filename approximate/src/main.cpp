#include <iostream>
#include <random>
#include <string>

#include "builders.h"
#include "ea.h"
#include "graph.h"
#include "modifiers.h"
#include "solvers.h"
#include "state.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <time_limit[s]>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string time_limit = argv[3];

    // start timer
    co::Timer timer(std::stoi(time_limit), 100);

    // build graph from input
    co::DGraph g(input_path);
    // g.print();

    // set configuration of ea
    co::ea::EvolutionConfiguration config(100, 4, 3, 1.0);

    // intialize random number generator with seed
    // co::Sampler sampler(g.V, config.population_size, 1);
    // initialize random number generator without seed
    co::Sampler sampler(g.V, config.population_size);

    co::State result = co::ea::evolve(g, config, sampler, timer);

    // state.save_solution(g, output_path);
    // result.println(g);

    return 0;
}
