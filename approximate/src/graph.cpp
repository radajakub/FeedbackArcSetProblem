#include "graph.h"

void co::Vertex::print(bool newLine) {
    std::cout << this->vertex << "(" << this->cost << ") ";
    if (newLine) std::cout << std::endl;
}

void co::Edge::print() {
    std::cout << this->source << " -(" << this->cost << ")-> " << this->target << std::endl;
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

    // preallocate adjacency lists and degree lists
    this->out_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->out_edges[i].reserve(this->V);

    this->in_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->in_edges[i].reserve(this->V);

    this->out_degrees.resize(this->V, 0);
    this->out_costs.resize(this->V, 0);
    this->in_degrees.resize(this->V, 0);
    this->in_costs.resize(this->V, 0);

    // compute adjacency lists and in/out degrees
    for (co::Edge &e : this->edges) {
        this->out_edges[e.source].push_back(co::Vertex(e.target, e.cost));
        this->in_edges[e.target].push_back(co::Vertex(e.source, e.cost));

        ++(this->out_degrees[e.source]);
        ++(this->in_degrees[e.target]);

        this->out_costs[e.source] += e.cost;
        this->in_costs[e.target] += e.cost;
    }
}

void co::DGraph::print() {
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;

    std::cout << " - Edges" << std::endl;
    for (int i = 0; i < this->V; ++i) {
        for (co::Vertex n : this->in_edges[i]) n.print(false);
        std::cout << "-[" << this->in_degrees[i] << "|" << this->in_costs[i] << "]-> ";
        std::cout << i;
        std::cout << " -[" << this->out_degrees[i] << "|" << this->out_costs[i] << "]-> ";
        for (co::Vertex n : this->out_edges[i]) n.print(false);
        std::cout << std::endl;
    }
}
