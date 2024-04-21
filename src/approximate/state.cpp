#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->cost = std::numeric_limits<int>::max();
    this->order.resize(V, 0);
}

void co::State::set_order(int vertex, int order) {
    this->order[vertex] = order;
}

void co::State::evaluate_full(co::DGraph &g) {
    this->cost = 0;
    // sum over all edges if the source is after the target in the ordering
    for (co::Edge &e : g.edges) {
        if (this->order[e.source] > this->order[e.target]) {
            this->cost += e.cost;
        }
    }
}

void co::State::evaluate_incremental(co::DGraph &g, co::State &original, std::vector<int> &changed) {
    std::vector<bool> changed_bool(g.V, false);
    for (int v : changed) {
        changed_bool[v] = true;
    }

    // compute changes only in the changed vertices
    for (int v : changed) {
        // subtract the original incoming and outgoing edges
        for (co::Vertex &u : g.out_edges[v]) {
            if (original.order[v] > original.order[u.vertex]) {
                this->cost -= u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && original.order[v] < original.order[u.vertex]) {
                this->cost -= u.cost;
            }
        }

        // add the new incoming and outgoing edges
        for (co::Vertex &u : g.out_edges[v]) {
            if (this->order[v] > this->order[u.vertex]) {
                this->cost += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && this->order[v] < this->order[u.vertex]) {
                this->cost += u.cost;
            }
        }
    }
}

void co::State::save_solution(co::DGraph &g, std::string &path) {
    std::ofstream f(path, std::ios_base::out);
    f << this->cost << std::endl;

    // for every vertex, print the edges that end in a vertex before in the ordering
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->order[start] > this->order[end.vertex]) {
                // add 1 to vertices to shift to 1-based indexing
                f << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;

    f.close();
}

void co::State::print_val() {
    std::cout << this->cost << std::endl;
}

void co::State::println(co::DGraph &g) {
    std::cout << "[" << this->cost << "] ";

    for (int o = 0; o < g.V; ++o) {
        for (int v = 0; v < g.V; ++v) {
            if (this->order[v] == o) {
                std::cout << v << " ";
            }
        }
    }

    std::cout << std::endl;
}
