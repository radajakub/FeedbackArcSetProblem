#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->value = -1;
    this->order.resize(V);
}

void co::State::random_init(std::default_random_engine &rng) {
    std::iota(this->order.begin(), this->order.end(), 0);
    std::shuffle(this->order.begin(), this->order.end(), rng);
}

void co::State::evaluate_full(co::DGraph &g) {
    this->value = 0;
    // sum over all outgoing edges for every vertex
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            // add it to removed only when the edge goes against the order
            if (this->order[start] > this->order[end.vertex]) {
                this->value += end.cost;
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

void co::State::println(co::DGraph &g) {
    std::cout << this->value << std::endl;

    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->order[start] > this->order[end.vertex]) {
                // add 1 to vertices to shift to 1-based
                std::cout << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;
}
