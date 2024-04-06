#include <iostream>
#include <random>
#include <string>

#include "graph.h"
#include "heuristic.h"
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

    // intialize random number generator
    co::Sampler sampler(0);

    // build graph from input
    co::DGraph g(input_path);

    co::State state(g.V);
    state.random_init(sampler.rng);
    state.evaluate_full(g);

    state.save_solution(g, output_path);

    std::cout << timer.elapsed_time() << std::endl;

    return 0;
}
