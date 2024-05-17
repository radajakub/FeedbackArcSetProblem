#include <omp.h>

#include <chrono>
#include <iostream>

#include "alns.h"
#include "exact.h"
#include "graph.h"
#include "save.h"
#include "utils.h"

const int EXACT_THRESHOLD = 6;
const int SAFETY_MARGIN_MS = 200;
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

    // split the components into small and big subgraphs
    std::vector<co::DGraph> small_subgraphs;
    std::vector<co::DGraph> big_subgraphs;
    int big_subgraphs_vertices = 0;
    for (co::DGraph &g : subgraphs) {
        if (g.V > EXACT_THRESHOLD) {
            big_subgraphs.push_back(g);
            big_subgraphs_vertices += g.V;
        } else {
            small_subgraphs.push_back(g);
        }
    }

    std::vector<co::State> small_solutions(small_subgraphs.size(), co::State(0));
    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < small_solutions.size(); ++i) {
        small_solutions[i] = co::branch_and_bound(small_subgraphs[i]);
    }

    std::vector<co::State> big_solutions(big_subgraphs.size(), co::State(0));
    std::chrono::milliseconds big_limit_ms = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
    std::chrono::milliseconds time_per_graph = big_limit_ms / big_subgraphs_vertices;
    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound
    for (int i = 0; i < big_subgraphs.size(); ++i) {
        std::cout << omp_get_thread_num() << std::endl;
        std::chrono::milliseconds graph_time = time_per_graph * big_subgraphs[i].V;
        std::chrono::steady_clock::time_point sub_deadline = std::chrono::steady_clock::now() + graph_time;
        co::ALNS alns(SEED, sub_deadline, big_subgraphs[i].V);
        co::State s = alns.solve(big_subgraphs[i]);
        big_solutions[i] = s;
    }

    // save solution to the output file
    co::save_solutions(small_solutions, small_subgraphs, big_solutions, big_subgraphs, output_path);

    return 0;
}
