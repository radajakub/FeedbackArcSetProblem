#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->cost = std::numeric_limits<int>::max();
    this->positions.resize(V, -1);
    this->max_position = 0;
}

// todo: paralellize this
void co::State::remove_vertex(int v, DGraph &g) {
    if (this->positions[v] == -1) {
        std::cout << "cannot remove the same vertex twice!!" << std::endl;
        exit(1);
    }

    int pos = this->positions[v];

    // decrease cost by all outgoing edges to the left of the removed vertex
    for (co::Vertex &u : g.out_edges[v]) {
        if (this->positions[u.vertex] != -1 && pos > this->positions[u.vertex]) {
            this->cost -= u.cost;
        }
    }

    // decrease cost by all incoming edges from the right of the removed vertex
    for (co::Vertex &u : g.in_edges[v]) {
        if (this->positions[u.vertex] != -1 && pos < this->positions[u.vertex]) {
            this->cost -= u.cost;
        }
    }

    // decrease positions of every node after the removed one
    for (int i = 0; i < g.V; ++i) {
        if (this->positions[i] > pos) {
            this->positions[i] -= 1;
        }
    }
    this->positions[v] = -1;
    this->max_position -= 1;
}

void co::State::place_vertex(int v, int pos, DGraph &g) {
    if (this->positions[v] != -1) {
        std::cout << "cannot insert already placed vertex!!" << std::endl;
        exit(1);
    }

    pos = std::min(pos, this->max_position + 1);

    // increase positions of all nodes after the placed one
    for (int i = 0; i < this->V; ++i) {
        if (this->positions[i] >= pos) {
            this->positions[i] += 1;
        }
    }
    this->positions[v] = pos;
    this->max_position += 1;

    // increase cost by all outgoing edges to the left of the placed vertex
    for (co::Vertex &u : g.out_edges[v]) {
        if (this->positions[u.vertex] != -1 && pos > this->positions[u.vertex]) {
            this->cost += u.cost;
        }
    }

    // increase cost by all incoming edges from the right of the placed vertex
    for (co::Vertex &u : g.in_edges[v]) {
        if (this->positions[u.vertex] != -1 && pos < this->positions[u.vertex]) {
            this->cost += u.cost;
        }
    }
}

std::vector<int> co::State::to_vertices() {
    std::vector<int> vertices(this->V);
    for (int i = 0; i < this->V; ++i) {
        vertices[this->positions[i]] = i;
    }
    return vertices;
}

void co::State::from_vertices(std::vector<int> &vertices) {
    for (int i = 0; i < this->V; ++i) {
        this->positions[vertices[i]] = i;
    }
}

void co::State::set_order(int v, int pos) {
    this->positions[v] = pos;
}

void co::State::set_order(std::vector<std::pair<int, int>> &indexed_vector) {
    for (int i = 0; i < indexed_vector.size(); ++i) {
        this->positions[indexed_vector[i].second] = i;
    }
}

// todo: paralellize this
void co::State::evaluate_full(co::DGraph &g) {
    this->max_position = g.V - 1;
    this->cost = 0;
    // sum over all edges if the source is after the target in the positions
    for (int u = 0; u < g.V; ++u) {
        // do not allow evaluation of partial states
        if (this->positions[u] == -1) {
            exit(1);
        }

        for (co::Vertex &v : g.out_edges[u]) {
            if (this->positions[u] > this->positions[v.vertex]) {
                this->cost += v.cost;
            }
        }
    }
}

void co::State::check_validtity(co::DGraph &g) {
    // check that it is indeed a permuatation
    std::vector<bool> visited(g.V, false);
    for (int i = 0; i < g.V; ++i) {
        if (this->positions[i] == -1) {
            std::cout << "Some order is -1 which should not be!!" << std::endl;
            exit(1);
        }

        if (visited[this->positions[i]]) {
            std::cout << "Some order is repeated which should not be!!" << std::endl;
            exit(1);
        }

        visited[this->positions[i]] = true;
    }

    bool found = false;
    std::vector<int> missing;
    for (int i = 0; i < g.V; ++i) {
        if (!visited[i]) {
            found = true;
            missing.push_back(i);
        }
    }
    if (found) {
        std::cout << "Some order is missing which should not be!!" << std::endl;
        std::cout << g.V << std::endl;
        std::cout << "missing: ";
        for (int i = 0; i < missing.size(); ++i) {
            std::cout << missing[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "positions: ";
        for (int i = 0; i < g.V; ++i) {
            std::cout << this->positions[i] << " ";
        }
        std::cout << std::endl;
        exit(1);
    }

    // check that the cost is correct
    co::State copy(*this);
    copy.evaluate_full(g);
    if (copy.cost != this->cost) {
        std::cout << "Cost is not correct!!" << std::endl;
        exit(1);
    }
}

void co::State::check_consistency(co::DGraph &g) {
    // check that the order is continuous ignoring all the -1s
    int count_present = 0;
    for (int i = 0; i < this->V; ++i) {
        if (this->positions[i] != -1) {
            count_present += 1;
        }
    }
    std::vector<bool> present(count_present, false);
    int max_pos = -1;
    for (int i = 0; i < this->V; ++i) {
        int pos = this->positions[i];
        max_pos = std::max(max_pos, pos);
        if (pos != -1) {
            if (pos < 0 || pos >= count_present) {
                std::cout << "Position is out of bounds!!" << std::endl;
                exit(1);
            }
            present[pos] = true;
        }
    }

    if (max_pos != count_present - 1) {
        std::cout << "Something wrong with the positions!!" << std::endl;
        exit(1);
    }

    for (bool p : present) {
        if (!p) {
            std::cout << "There is a gap in the positions!!" << std::endl;
            exit(1);
        }
    }
}

void co::State::print(std::string prefix) {
    std::cout << prefix << " [" << this->cost << "] ";

    for (int i = 0; i < this->V; ++i) {
        std::cout << this->positions[i] << " ";
    }

    std::cout << std::endl
              << std::endl;
}

void co::State::save_solution(co::DGraph &g, std::ofstream &f) {
    // for every vertex, print the edges that end in a vertex before in the positions
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->positions[start] > this->positions[end.vertex]) {
                // add 1 to vertices to shift to 1-based indexing
                f << g.vertex_map[start] + 1 << " " << g.vertex_map[end.vertex] + 1 << std::endl;
            }
        }
    }
}

co::IncrementalState::IncrementalState(int V) : V(V), cost(0), placed(0) {
    this->positions.resize(V, -1);
}

void co::IncrementalState::place_vertex(int vertex, co::DGraph &g) {
    // place the vertex
    this->positions[vertex] = this->placed++;

    // add outgoing edge costs
    for (co::Vertex &u : g.out_edges[vertex]) {
        if (this->positions[u.vertex] != -1 && this->positions[vertex] > this->positions[u.vertex]) {
            this->cost += u.cost;
        }
    }

    for (co::Vertex &u : g.in_edges[vertex]) {
        if (this->positions[u.vertex] != -1 && this->positions[u.vertex] > this->positions[vertex]) {
            this->cost += u.cost;
        }
    }
}

int co::IncrementalState::cheapest_cost(co::DGraph &g, std::vector<int> &vertices) {
    int min_cost = 0;
    for (int vertex : vertices) {
        int subcost = 0;

        for (co::Vertex &u : g.out_edges[vertex]) {
            if (this->positions[u.vertex] != -1 && this->positions[vertex] > this->positions[u.vertex]) {
                subcost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[vertex]) {
            if (this->positions[u.vertex] != -1 && this->positions[u.vertex] > this->positions[vertex]) {
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
    state.positions = this->positions;
    return state;
}

void co::IncrementalState::print_val() {
    std::cout << this->cost << std::endl;
}

void co::IncrementalState::print_ordering() {
    std::cout << "[" << this->cost << "] ";

    for (int v : this->positions) {
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
        vertices[this->positions[i]] = i;
    }
    return vertices;
}
