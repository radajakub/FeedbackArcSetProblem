#include <omp.h>

#include <chrono>
#include <iostream>

#include "alns.h"
#include "graph.h"
#include "save.h"
#include "utils.h"

const int SAFETY_MARGIN_MS = 100;
const int SEED = -1;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <time_limit[s]>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string time_limit = argv[3];

    // start timer
    std::chrono::milliseconds limit_ms(static_cast<int>(std::floor(std::stof(time_limit)) * 1000) - SAFETY_MARGIN_MS);
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + limit_ms;

    co::InputGraph input_graph(input_path);
    std::vector<co::DGraph> subgraphs = input_graph.condense();
    int n = 0;
    for (co::DGraph &g : subgraphs) {
        n += g.V;
    }

    std::vector<co::State> solutions;
    std::chrono::milliseconds big_limit_ms = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
    std::chrono::milliseconds time_per_graph = big_limit_ms / n;

    for (co::DGraph &g : subgraphs) {
        std::chrono::milliseconds graph_time = time_per_graph * g.V;
        std::chrono::steady_clock::time_point sub_deadline = std::chrono::steady_clock::now() + graph_time;
        co::ALNS alns(SEED, sub_deadline, g.V);
        co::State s = alns.solve(g);
        solutions.push_back(s);
    }

    // save solution to the output file
    co::save_solutions(solutions, subgraphs, output_path);

    return 0;
}
