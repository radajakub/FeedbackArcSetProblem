#include "gurobi_c++.h"

using namespace std;

int main(int argc, char *argv[])
{
    GRBEnv env;
    GRBModel model(env);

    GRBVar x = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x");
    GRBVar y = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "y");

    model.setObjective(32 * x + 25 * y, GRB_MAXIMIZE);
    model.addConstr(5 * x + 4 * y <= 59, "cons1");
    model.addConstr(4 * x + 3 * y <= 46, "cons2");

    model.optimize();

    cout << "Optimal objective: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
    cout << "x: " << x.get(GRB_DoubleAttr_X) << endl;
    cout << "y: " << y.get(GRB_DoubleAttr_X) << endl;

    return 0;
}
