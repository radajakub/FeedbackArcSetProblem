#include "milp.h"

co::Solution co::solve_milp(co::DGraph &g) {
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

    // free variables
    delete[] x;

    co::Solution solution(g.edges);
    return solution;
}
