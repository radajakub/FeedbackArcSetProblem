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
    for (int u = 0; u < g.V; ++u) {
        // allow evaluation of partial states
        if (this->ordering[u] == -1) {
            this->is_full = false;
            continue;
        }

        for (co::Vertex &v : g.out_edges[u]) {
            if (this->ordering[u] > this->ordering[v.vertex]) {
                this->cost += v.cost;
            }
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

void co::State::save_solution(co::DGraph &g, std::ofstream &f) {
    // for every vertex, print the edges that end in a vertex before in the ordering
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->ordering[start] > this->ordering[end.vertex]) {
                // add 1 to vertices to shift to 1-based indexing
                f << g.vertex_map[start] + 1 << " " << g.vertex_map[end.vertex] + 1 << std::endl;
            }
        }
    }
}

co::IncrementalState::IncrementalState(int V) : V(V), cost(0), placed(0) {
    this->ordering.resize(V, -1);
}

void co::IncrementalState::place_vertex(int vertex, co::DGraph &g) {
    // place the vertex
    this->ordering[vertex] = this->placed++;

    // add outgoing edge costs
    for (co::Vertex &u : g.out_edges[vertex]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[vertex] > this->ordering[u.vertex]) {
            this->cost += u.cost;
        }
    }

    for (co::Vertex &u : g.in_edges[vertex]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[u.vertex] > this->ordering[vertex]) {
            this->cost += u.cost;
        }
    }
}

int co::IncrementalState::cheapest_cost(co::DGraph &g, std::vector<int> &vertices) {
    int min_cost = 0;
    for (int vertex : vertices) {
        int subcost = 0;

        for (co::Vertex &u : g.out_edges[vertex]) {
            if (this->ordering[u.vertex] != -1 && this->ordering[vertex] > this->ordering[u.vertex]) {
                subcost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[vertex]) {
            if (this->ordering[u.vertex] != -1 && this->ordering[u.vertex] > this->ordering[vertex]) {
                subcost += u.cost;
            }
        }

        min_cost = std::min(min_cost, subcost);
    }

    return min_cost;
}

co::State co::IncrementalState::to_state() {
    co::State state(this->V);
    state.cost = this->cost;
    state.ordering = this->ordering;
    return state;
}

void co::IncrementalState::print_val() {
    std::cout << this->cost << std::endl;
}

void co::IncrementalState::print_ordering() {
    std::cout << "[" << this->cost << "] ";

    for (int v : this->ordering) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}

void co::IncrementalState::print_vertices() {
    std::cout << "[" << this->cost << "] ";

    for (int v : this->to_vertices()) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}

std::vector<int> co::IncrementalState::to_vertices() {
    std::vector<int> vertices(this->V);
    for (int i = 0; i < this->V; ++i) {
        vertices[this->ordering[i]] = i;
    }
    return vertices;
}
