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
    std::chrono::milliseconds limit_ms(std::stoi(time_limit) * 1000 - SAFETY_MARGIN_MS);
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + limit_ms;

    co::InputGraph input_graph(input_path);
    std::vector<co::DGraph> subgraphs = input_graph.condense();

    // split the components into small and big subgraphs
    std::vector<co::DGraph> small_subgraphs;
    std::vector<co::DGraph> big_subgraphs;
    for (co::DGraph &g : subgraphs) {
        if (g.V > EXACT_THRESHOLD) {
            big_subgraphs.push_back(g);
        } else {
            small_subgraphs.push_back(g);
        }
    }

    // std::cout << "original graph: |V| = " << input_graph.V << " |E| = " << input_graph.E << std::endl;
    // std::cout << "subgraphs: " << subgraphs.size() << std::endl;
    // std::cout << "small subgraphs: " << small_subgraphs.size() << std::endl;
    // std::cout << "big subgraphs: " << big_subgraphs.size() << std::endl;

    std::vector<co::State> small_solutions;
    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound
    for (co::DGraph &g : small_subgraphs) {
        small_solutions.push_back(co::branch_and_bound(g));
    }

    std::vector<co::State> big_solutions;
    std::chrono::milliseconds big_limit_ms = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
    std::chrono::milliseconds time_per_graph = big_limit_ms / big_subgraphs.size();
    std::chrono::steady_clock::time_point sub_deadline = std::chrono::steady_clock::now() + time_per_graph;
    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound
    for (int i = 0; i < big_subgraphs.size(); ++i) {
        std::cout << "solving " << i << std::endl;
        co::ALNS alns(SEED, sub_deadline);
        co::State s = alns.solve(big_subgraphs[i]);
        big_solutions.push_back(s);
        sub_deadline += time_per_graph;
    }

    // save solution to the output file
    co::save_solutions(small_solutions, small_subgraphs, big_solutions, big_subgraphs, output_path);

    return 0;
}
