#include "milp.h"

co::Solution co::solve_milp(co::DGraph &g) {
    GRBEnv env;
    GRBModel model(env);

    // represent edges to remove from the graph
    GRBVar *x = model.addVars(g.E, GRB_BINARY);
    // represent topological ordering of vertices
    GRBVar *y = model.addVars(g.V, GRB_INTEGER);

    // set objective as a sum of edge costs for edges which are in the graph
    GRBLinExpr expression = 0;
    for (int i = 0; i < g.E; ++i) {
        co::Edge e = g.edges[i];
        expression += x[i] * e.cost;
    }
    model.setObjective(expression, GRB_MINIMIZE);

    // add constraint to enforce topological ordering of vertices currently in the graph
    for (int i = 0; i < g.E; ++i) {
        co::Edge e = g.edges[i];
        model.addConstr(y[e.source] + 1 <= y[e.target] + g.V * x[i]);
    }

    model.optimize();

    // extract edges which have not been deleted
    std::vector<co::Edge> remaining_edges;
    remaining_edges.reserve(g.E);
    for (int i = 0; i < g.E; ++i) {
        if (x[i].get(GRB_DoubleAttr_X) > 0.5) {
            remaining_edges.push_back(g.edges[i]);
        }
    }

    co::Solution solution(remaining_edges);

    // free variables
    delete[] x;
    delete[] y;

    return solution;
}
