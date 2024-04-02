#include <iostream>
#include <string>

#include "graph.h"
#include "heuristic.h"
#include "state.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::random_device rd;
    std::default_random_engine rng(rd());

    co::DGraph g(input_path);
    // g.print();

    co::State state(g.V);
    state.random_init(rng);
    state.evaluate_full(g);

    state.println(g);
    // state.save_solution(g, output_path);

    return 0;
}
