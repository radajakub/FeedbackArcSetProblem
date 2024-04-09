#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->value = -1;
    this->order.resize(V);
}

void co::State::set_order(int vertex, int order) {
    this->order[vertex] = order;
}

void co::State::evaluate_full(co::DGraph &g) {
    this->value = 0;
    // sum over all outgoing edges for every vertex
    for (co::Edge &e : g.edges) {
        if (this->order[e.source] > this->order[e.target]) {
            this->value += e.cost;
        }
    }
}

void co::State::evaluate_incremental(co::DGraph &g, co::State &original, std::vector<int> &changed) {
    std::vector<bool> changed_bool(g.V, false);
    for (int v : changed) {
        changed_bool[v] = true;
    }

    for (int v : changed) {
        // subtract the original incoming and outgoing edges
        for (co::Vertex &u : g.out_edges[v]) {
            if (original.order[v] > original.order[u.vertex]) {
                this->value -= u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && original.order[v] < original.order[u.vertex]) {
                this->value -= u.cost;
            }
        }
        for (co::Vertex &u : g.out_edges[v]) {
            if (this->order[v] > this->order[u.vertex]) {
                this->value += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && this->order[v] < this->order[u.vertex]) {
                this->value += u.cost;
            }
        }
    }
}

void co::State::save_solution(co::DGraph &g, std::string &path) {
    std::ofstream f(path, std::ios_base::out);
    f << this->value << std::endl;

    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->order[start] > this->order[end.vertex]) {
                // add 1 to vertices to shift to 1-based
                f << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;

    f.close();
}

void co::State::print_val() {
    std::cout << this->value << std::endl;
}

void co::State::println(co::DGraph &g) {
    std::cout << "[" << this->value << "] ";

    for (int o = 0; o < g.V; ++o) {
        for (int v = 0; v < g.V; ++v) {
            if (this->order[v] == o) {
                std::cout << v << " ";
            }
        }
    }

    std::cout << std::endl;
}
