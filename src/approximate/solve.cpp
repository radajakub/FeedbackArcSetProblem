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

    // build graph from input
    co::DGraph g(input_path);

    // intialize random number generator
    std::cout << g.V << std::endl;
    co::Sampler sampler(g.V, 1);

    co::State state(g.V);
    state.random_init(sampler);
    // state.degree_init(g);
    state.evaluate_full(g);

    while (!timer.should_stop()) {
        co::State candidate(state);
        candidate.random_init(sampler);
        candidate.evaluate_full(g);
        // candidate.perturbate(sampler);
        // candidate.evaluate_full(g);

        if (candidate.value < state.value) {
            state = co::State(candidate);
            std::cout << candidate.value << std::endl;
        }
    }

    state.save_solution(g, output_path);

    return 0;
}
