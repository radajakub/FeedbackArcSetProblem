#include <iostream>

#include "exact.h"
#include "graph.h"
#include "hill_climber.h"
#include "utils.h"

int EXACT_THRESHOLD = 5;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <time_limit[s]>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string time_limit = argv[3];

    // start timer
    co::Timer timer(std::stoi(time_limit), 200);

    co::Sampler sampler;

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

    std::vector<co::State> small_solutions;
    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound
    for (co::DGraph &g : small_subgraphs) {
        small_solutions.push_back(co::branch_and_bound(g));
    }

    std::vector<co::State> big_solutions;

    // if there are subgraphs that are smaller than some threshold, solve them exactly by branch and bound
    for (int i = 0; i < big_subgraphs.size(); ++i) {
        // todo: fix timers (it works now because there is always only one graph to solve approximately)
        co::State s = co::hill_climber(big_subgraphs[i], timer, sampler);
        big_solutions.push_back(s);
    }

    std::ofstream output_file(output_path, std::ios_base::out);

    int total_cost = 0;
    for (co::State &s : small_solutions) {
        total_cost += s.cost;
    }
    for (co::State &s : big_solutions) {
        total_cost += s.cost;
    }

    output_file << total_cost << std::endl;

    for (int i = 0; i < small_solutions.size(); ++i) {
        small_solutions[i].save_solution(small_subgraphs[i], output_file);
    }

    for (int i = 0; i < big_solutions.size(); ++i) {
        big_solutions[i].save_solution(big_subgraphs[i], output_file);
    }

    output_file.close();

    return 0;
}
