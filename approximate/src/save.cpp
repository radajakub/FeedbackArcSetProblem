#include "save.h"

void co::save_solutions(std::vector<co::State> &small_solutions, std::vector<co::DGraph> &small_gs, std::vector<co::State> &big_solutions, std::vector<co::DGraph> &big_gs, std::string outpath) {
    std::ofstream output_file(outpath, std::ios_base::out);

    int total_cost = 0;
    for (co::State &s : small_solutions) {
        total_cost += s.cost;
    }
    for (co::State &s : big_solutions) {
        total_cost += s.cost;
    }

    output_file << total_cost << std::endl;

    for (int i = 0; i < small_solutions.size(); ++i) {
        small_solutions[i].save_solution(small_gs[i], output_file);
    }
    for (int i = 0; i < big_solutions.size(); ++i) {
        big_solutions[i].save_solution(big_gs[i], output_file);
    }

    output_file.close();
}
