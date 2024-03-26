#include <iostream>
#include <string>

#include "graph.h"
#include "heuristic.h"
#include "solution.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::vector<co::Edge> edges = co::load_edges(input_path);

    co::DGraph g(edges);

    co::Solution solution = co::solve_heuristic(g);

    solution.save(output_path);

    return 0;
}
