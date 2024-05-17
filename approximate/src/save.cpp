#include "save.h"

void co::save_solutions(std::vector<co::State> &solutions, std::vector<co::DGraph> &gs, std::string outpath) {
    std::ofstream output_file(outpath, std::ios_base::out);

    int total_cost = 0;
    for (co::State &s : solutions) {
        total_cost += s.cost;
    }

    output_file << total_cost << std::endl;

    for (int i = 0; i < solutions.size(); ++i) {
        solutions[i].save_solution(gs[i], output_file);
    }

    output_file.close();
}
