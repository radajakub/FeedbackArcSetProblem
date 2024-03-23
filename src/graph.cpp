#include "graph.hpp"

co::DGraph::DGraph(std::vector<co::Edge> &edges) {
    this->E = edges.size();

    // find max V
    this->V = 0;
    for (co::Edge e : edges)
        this->V = std::max(this->V, std::max(e.source, e.target));

    ++this->V;

    this->edges = edges;

    // create adjacency list [V x Ni] and cost matrix [V x V]
    this->adjacency_list = std::vector<std::vector<int>>(this->V);
    for (int i = 0; i < this->V; ++i) this->adjacency_list[i].reserve(this->V);

    this->adjacency_matrix = std::vector<std::vector<int>>(this->V, std::vector<int>(this->V, 0));
    for (co::Edge e : edges) {
        this->adjacency_list[e.source].push_back(e.target);
        this->adjacency_matrix[e.source][e.target] = e.cost;
    }
}

void co::DGraph::print() {
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;
    std::cout << " - Adjacency List" << std::endl;
    for (int v1 = 0; v1 < this->V; ++v1) {
        std::cout << " --- " << v1 << " -> ";
        for (int v2 : this->adjacency_list[v1]) {
            std::cout << v2 << "(" << this->adjacency_matrix[v1][v2] << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<co::Edge> co::load_edges(std::string path) {
    std::ifstream f(path, std::ios_base::in);
    int E;
    f >> E;

    std::vector<co::Edge> edges;

    int source, target, cost;

    for (int i = 0; i < E; ++i) {
        f >> source >> target >> cost;
        // shift to start at 0
        edges.push_back(co::Edge(source - 1, target - 1, cost));
    }

    f.close();

    return edges;
}

void co::print_vector(std::vector<int> &v) {
    for (int i = 0; i < v.size(); ++i) std::cout << v[i] << " ";
    std::cout << std::endl;
}

void co::print_matrix(std::vector<std::vector<int>> &m) {
    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            std::cout << std::setw(2) << std::setfill(' ') << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
