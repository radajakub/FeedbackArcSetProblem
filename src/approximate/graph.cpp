#include "graph.h"

void co::Vertex::print(bool newLine) {
    std::cout << this->vertex << "(" << this->cost << ") ";
    if (newLine) std::cout << std::endl;
}

co::DGraph::DGraph(std::string &path) {
    std::ifstream f(path, std::ios_base::in);

    f >> this->E;

    std::vector<std::tuple<int, int, int>> edges;

    this->V = 0;
    int source, target, cost;

    for (int i = 0; i < E; ++i) {
        f >> source >> target >> cost;
        // shift to start at 0
        edges.push_back(std::make_tuple(source - 1, target - 1, cost));
        this->V = std::max(this->V, std::max(source, target));
    }
    f.close();

    this->out_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->out_edges[i].reserve(this->V);

    this->in_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->in_edges[i].reserve(this->V);

    for (std::tuple<int, int, int> &e : edges) {
        int source = std::get<0>(e);
        int target = std::get<1>(e);
        int cost = std::get<2>(e);
        this->out_edges[source].push_back(co::Vertex(target, cost));
        this->in_edges[target].push_back(co::Vertex(source, cost));
    }
}

void co::DGraph::print() {
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;

    std::cout << " - Adjacency Lists" << std::endl;
    for (int i = 0; i < this->V; ++i) {
        for (co::Vertex n : this->in_edges[i]) n.print(false);
        std::cout << " -> " << i << " -> ";
        for (co::Vertex n : this->out_edges[i]) n.print(false);
        std::cout << std::endl;
    }
}
