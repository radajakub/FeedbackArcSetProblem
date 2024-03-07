#include <iostream>
#include "graph.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::vector<std::vector<int>> edges = co::load_edges(input_path);

    co::DGraph g(edges);
    g.print();

    return 0;
}
