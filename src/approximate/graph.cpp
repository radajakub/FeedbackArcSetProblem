#include "graph.h"

void co::Edge::println() {
    std::cout << this->source << " -(" << this->cost << ")-> " << this->target << std::endl;
}

void co::Node::print(bool newLine) {
    std::cout << this->node << "(" << this->cost << ") ";
    if (newLine) std::cout << std::endl;
}

co::DGraph::DGraph(std::vector<co::Edge> &edges) {
    this->E = edges.size();

    // find max V
    this->V = 0;
    for (co::Edge e : edges)
        this->V = std::max(this->V, std::max(e.source, e.target));

    ++this->V;

    this->edges = edges;

    this->out_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->out_edges[i].reserve(this->V);

    this->in_edges.resize(this->V);
    for (int i = 0; i < this->V; ++i)
        this->in_edges[i].reserve(this->V);

    for (co::Edge e : edges) {
        this->out_edges[e.source].push_back(co::Node(e.target, e.cost));
        this->in_edges[e.target].push_back(co::Node(e.source, e.cost));
    }
}

void co::DGraph::print() {
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;

    std::cout << " - Edges" << std::endl;
    for (co::Edge e : this->edges) e.println();

    std::cout << " - Adjacency Lists" << std::endl;
    for (int i = 0; i < this->V; ++i) {
        for (co::Node n : this->in_edges[i]) n.print(false);
        std::cout << " -> " << i << " -> ";
        for (co::Node n : this->out_edges[i]) n.print(false);
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
