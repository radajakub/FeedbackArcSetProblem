#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->cost = std::numeric_limits<int>::max();
    this->ordering.resize(V, -1);
}

void co::State::set_order(int vertex, int order) {
    this->ordering[vertex] = order;
}

void co::State::set_order(std::vector<std::pair<int, int>> &indexed_vector) {
    for (int i = 0; i < indexed_vector.size(); ++i)
        this->ordering[indexed_vector[i].second] = i;
}

std::vector<int> co::State::to_vertices() {
    std::vector<int> vertices(this->V);
    for (int i = 0; i < this->V; ++i) {
        vertices[this->ordering[i]] = i;
    }
    return vertices;
}

void co::State::set_vertices(std::vector<int> &vertices) {
    for (int i = 0; i < this->V; ++i) {
        this->ordering[vertices[i]] = i;
    }
}

void co::State::evaluate_full(co::DGraph &g) {
    this->cost = 0;
    // sum over all edges if the source is after the target in the ordering
    for (co::Edge &e : g.edges) {
        if (this->ordering[e.source] > this->ordering[e.target]) {
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
            if (original.ordering[v] > original.ordering[u.vertex]) {
                this->cost -= u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && original.ordering[v] < original.ordering[u.vertex]) {
                this->cost -= u.cost;
            }
        }

        // add the new incoming and outgoing edges
        for (co::Vertex &u : g.out_edges[v]) {
            if (this->ordering[v] > this->ordering[u.vertex]) {
                this->cost += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (!changed_bool[u.vertex] && this->ordering[v] < this->ordering[u.vertex]) {
                this->cost += u.cost;
            }
        }
    }
}

void co::State::print_val() {
    std::cout << this->cost << std::endl;
}

void co::State::print_ordering() {
    std::cout << "[" << this->cost << "] ";

    for (int v : this->ordering) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}

void co::State::print_vertices() {
    std::cout << "[" << this->cost << "] ";

    for (int v : this->to_vertices()) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}

void co::State::save_solution(co::DGraph &g, std::string &path) {
    std::ofstream f(path, std::ios_base::out);
    f << this->cost << std::endl;

    // for every vertex, print the edges that end in a vertex before in the ordering
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->ordering[start] > this->ordering[end.vertex]) {
                // add 1 to vertices to shift to 1-based indexing
                f << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;

    f.close();
}
