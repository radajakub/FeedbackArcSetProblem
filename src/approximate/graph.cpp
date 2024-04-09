#include "graph.h"

void co::Vertex::print(bool newLine) {
    std::cout << this->vertex << "(" << this->cost << ") ";
    if (newLine) std::cout << std::endl;
}

co::DGraph::DGraph(std::string &path) {
    std::ifstream f(path, std::ios_base::in);

    f >> this->E;

    this->edges.reserve(this->E);

    this->V = 0;
    int source, target, cost;

    for (int i = 0; i < E; ++i) {
        f >> source >> target >> cost;
        // shift to start at 0
        this->edges.push_back(co::Edge(source - 1, target - 1, cost));
        this->V = std::max(this->V, std::max(source, target));
    }
    f.close();

    this->out_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->out_edges[i].reserve(this->V);

    this->in_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->in_edges[i].reserve(this->V);

    for (co::Edge &e : this->edges) {
        this->out_edges[e.source].push_back(co::Vertex(e.target, e.cost));
        this->in_edges[e.target].push_back(co::Vertex(e.source, e.cost));
    }

    // compute degrees
    this->out_degrees.resize(this->V);
    this->in_degrees.resize(this->V);
    for (int v = 0; v < this->V; ++v) {
        this->out_degrees[v] = this->out_edges[v].size();
        this->in_degrees[v] = this->in_edges[v].size();
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
