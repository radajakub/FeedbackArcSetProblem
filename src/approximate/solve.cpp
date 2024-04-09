#include <iostream>
#include <random>
#include <string>

#include "builders.h"
#include "graph.h"
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
    co::Sampler sampler(g.V, 1);

    // co::State state = co::builders::random(g, sampler);
    // co::State state = co::builders::in_degree(g);
    co::State state = co::builders::out_degree(g);

    state.evaluate_full(g);

    // state.save_solution(g, output_path);
    state.println(g);

    return 0;
}
