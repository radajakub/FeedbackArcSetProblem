#include "exact.h"

co::State co::branch_and_bound(co::DGraph &g) {
    int best_cost = std::numeric_limits<int>::max();
    co::IncrementalState best_state(g.V);

    // prepare list of all vertices
    std::vector<std::pair<int, int>> positions = g.in_cost_vertices();
    std::vector<int> vertices(g.V);
    for (int i = 0; i < positions.size(); ++i) {
        vertices[i] = positions[i].second;
    }

    std::reverse(vertices.begin(), vertices.end());

    std::stack<co::IncrementalState> stack;
    stack.push(best_state);

    while (!stack.empty()) {
        // pop current state
        co::IncrementalState current = stack.top();
        stack.pop();

        // check if the solution is full
        if (g.V == current.placed) {
            // save solution if it is better
            if (current.cost < best_cost) {
                best_cost = current.cost;
                best_state = current;
            }
            continue;
        }

        int most_expensive = current.cheapest_cost(g, vertices);
        if (current.cost + most_expensive > best_cost) {
            continue;
        }

        // try assigning all unassigned neighbors
        for (int v : vertices) {
            // skip if it was already assigned
            if (current.positions[v] != -1) continue;

            co::IncrementalState next = current;
            next.place_vertex(v, g);
            stack.push(next);
        }
    }

    return best_state.to_state();
}
