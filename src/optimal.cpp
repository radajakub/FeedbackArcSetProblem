#include <gurobi_c++.h>

#include <iostream>

#include "graph.hpp"

int solve(co::DGraph &g) {
    GRBEnv env;
    GRBModel model(env);

    GRBVar *x = model.addVars(g.E, GRB_BINARY);

    // set objective as a sum of edge costs for edges which are in the graph
    GRBLinExpr expression = 0;
    for (int i = 0; i < g.E; ++i) {
        co::Edge e = g.edges[i];
        expression += x[i] * e.cost;
    }
    model.setObjective(expression, GRB_MINIMIZE);

    model.optimize();

    int obj_value = model.get(GRB_DoubleAttr_ObjVal);

    // free variables
    delete[] x;

    return obj_value;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    std::vector<co::Edge> edges = co::load_edges(input_path);

    co::DGraph g(edges);
    g.print();
    std::cout << std::endl;
    co::print_matrix(g.adjacency_matrix);

    solve(g);

    return 0;
}
