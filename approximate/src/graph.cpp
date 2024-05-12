#include "./graph.h"

void co::Edge::print() {
    std::cout << this->source << " -(" << this->cost << ")-> " << this->target << std::endl;
}

void co::Vertex::print() {
    std::cout << this->vertex << "(" << this->cost << ")";
}

co::DGraph::DGraph(int V, std::vector<Edge> &edges, std::vector<int> &vertex_map) {
    this->V = V;
    this->edges = edges;
    this->E = edges.size();
    this->vertex_map = vertex_map;

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

std::vector<std::pair<int, int>> co::DGraph::out_cost_vertices() {
    std::vector<std::pair<int, int>> indexed_list = co::enumerate(this->out_costs);

    std::sort(indexed_list.begin(), indexed_list.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return this->in_costs[a.second] < this->in_costs[b.second];
        }
        return a.first > b.first;
    });

    return indexed_list;
}

std::vector<std::pair<int, int>> co::DGraph::in_cost_vertices() {
    std::vector<std::pair<int, int>> indexed_list = co::enumerate(this->in_costs);

    std::sort(indexed_list.begin(), indexed_list.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return this->out_degrees[a.second] > this->out_degrees[b.second];
        }
        return a.first < b.first;
    });

    return indexed_list;
}

std::vector<std::pair<int, int>> co::DGraph::out_degree_vertices() {
    std::vector<std::pair<int, int>> indexed_list = co::enumerate(this->out_costs);

    std::sort(indexed_list.begin(), indexed_list.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return this->in_degrees[a.second] < this->in_degrees[b.second];
        }
        return a.first > b.first;
    });

    return indexed_list;
}

std::vector<std::pair<int, int>> co::DGraph::in_degree_vertices() {
    std::vector<std::pair<int, int>> indexed_list = co::enumerate(this->out_costs);

    std::sort(indexed_list.begin(), indexed_list.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return this->out_degrees[a.second] > this->out_degrees[b.second];
        }
        return a.first < b.first;
    });

    return indexed_list;
}

void co::DGraph::print() {
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;

    std::cout << " - Edges" << std::endl;
    for (co::Edge &e : this->edges) {
        e.print();
    }
}

co::InputGraph::InputGraph(std::string &path) {
    std::ifstream f(path, std::ios_base::in);
    f >> this->E;

    this->edges.reserve(E);

    int source, target, cost;
    for (int i = 0; i < E; ++i) {
        f >> source >> target >> cost;
        // shift to start at 0
        this->edges.push_back(co::Edge(source - 1, target - 1, cost));
        this->V = std::max(this->V, std::max(source, target));
    }

    f.close();
}

std::vector<co::DGraph> co::InputGraph::condense() {
    std::vector<co::DGraph> subgraphs;

    int index = 1;
    std::stack<int> stack;

    std::vector<int> indices(this->V, -1);
    std::vector<int> lowlinks(this->V, -1);
    std::vector<bool> onstacks(this->V, false);

    for (int i = 0; i < this->V; ++i) {
        if (indices[i] == -1) {
            this->strong_connect(i, index, stack, indices, lowlinks, onstacks, subgraphs);
        }
    }

    return subgraphs;
}

void co::InputGraph::strong_connect(int v, int &index, std::stack<int> &stack, std::vector<int> &indices, std::vector<int> &lowlinks, std::vector<bool> &onstacks, std::vector<co::DGraph> &subgraphs) {
    indices[v] = index;
    lowlinks[v] = index;
    ++index;

    stack.push(v);
    onstacks[v] = true;

    for (co::Edge &e : this->edges) {
        if (e.source != v) continue;

        int w = e.target;
        if (indices[w] == -1) {
            this->strong_connect(w, index, stack, indices, lowlinks, onstacks, subgraphs);
            lowlinks[v] = std::min(lowlinks[v], lowlinks[w]);
        } else if (onstacks[w]) {
            lowlinks[v] = std::min(lowlinks[v], indices[w]);
        }
    }

    if (lowlinks[v] == indices[v]) {
        std::vector<int> subvertices;
        while (true) {
            int w = stack.top();
            stack.pop();

            onstacks[w] = false;
            subvertices.push_back(w);

            if (w == v) break;
        }

        // ignore subgraphs of size 1
        int subV = subvertices.size();
        if (subV > 1) {
            // output subgraph
            std::vector<co::Edge> subedges;
            std::vector<int> vertex_map(subV);
            std::vector<int> inverse_vertex_map(this->V);
            std::vector<bool> is_subvertex(this->V, false);
            for (int i = 0; i < subV; ++i) {
                is_subvertex[subvertices[i]] = true;
                vertex_map[i] = subvertices[i];
                inverse_vertex_map[subvertices[i]] = i;
            }

            for (co::Edge &e : this->edges) {
                if (is_subvertex[e.source] && is_subvertex[e.target]) {
                    subedges.push_back(co::Edge(inverse_vertex_map[e.source], inverse_vertex_map[e.target], e.cost));
                }
            }
            subgraphs.push_back(co::DGraph(subV, subedges, vertex_map));
        }
    }
}

void co::InputGraph::print() {
    std::cout << "Input Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;

    std::cout << " - Edges" << std::endl;
    for (co::Edge &e : this->edges) {
        e.print();
    }
}
