#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->cost = std::numeric_limits<int>::max();
    this->ordering.resize(V, -1);
}

void co::State::remove_vertex(int v, DGraph &g) {
    // decrease cost by all outgoing edges to the left of the removed vertex
    for (co::Vertex &u : g.out_edges[v]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[v] > this->ordering[u.vertex]) {
            this->cost -= u.cost;
        }
    }

    // decrease cost by all incoming edges from the right of the removed vertex
    for (co::Vertex &u : g.in_edges[v]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[v] < this->ordering[u.vertex]) {
            this->cost -= u.cost;
        }
    }

    // decrease ordering of every node after the removed one
    for (int i = 0; i < this->V; ++i) {
        if (this->ordering[i] > this->ordering[v]) {
            --this->ordering[i];
        }
    }
    this->ordering[v] = -1;
}

void co::State::place_vertex(int v, int pos, DGraph &g) {
    // increase ordering of all nodes after the placed one
    for (int i = 0; i < this->V; ++i) {
        if (this->ordering[i] >= pos) {
            ++this->ordering[i];
        }
    }
    this->ordering[v] = pos;

    // increase cost by all outgoing edges to the left of the placed vertex
    for (co::Vertex &u : g.out_edges[v]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[v] > this->ordering[u.vertex]) {
            this->cost += u.cost;
        }
    }

    // increase cost by all incoming edges from the right of the placed vertex
    for (co::Vertex &u : g.in_edges[v]) {
        if (this->ordering[u.vertex] != -1 && this->ordering[v] < this->ordering[u.vertex]) {
            this->cost += u.cost;
        }
    }
}

void co::State::set_order(int v, int pos) {
    this->ordering[v] = pos;
}

void co::State::set_order(std::vector<std::pair<int, int>> &indexed_vector) {
    for (int i = 0; i < indexed_vector.size(); ++i) {
        this->ordering[indexed_vector[i].second] = i;
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

void co::State::print(std::string prefix) {
    std::cout << prefix << " [" << this->cost << "] ";

    for (int v : this->ordering) {
        std::cout << v << " ";
    }

    std::cout << std::endl
              << std::endl;
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
