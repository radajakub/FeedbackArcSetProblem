#include "state.h"

co::State::State(int V) {
    this->V = V;
    this->value = -1;
    this->order.resize(V);
}

co::State::State(co::State &s) {
    this->V = s.V;
    this->value = s.value;
    this->order = s.order;
}

void co::State::random_init(co::Sampler &sampler) {
    std::iota(this->order.begin(), this->order.end(), 0);
    std::shuffle(this->order.begin(), this->order.end(), sampler.rng);
}

void co::State::degree_init(co::DGraph &g) {
    std::vector<std::pair<int, int>> enhanced_indegrees(g.V);
    for (int i = 0; i < g.V; ++i) {
        enhanced_indegrees[i] = std::make_pair(g.in_degrees[i], i);
    }
    // sort by in-degrees
    std::sort(enhanced_indegrees.begin(), enhanced_indegrees.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.first == b.first) {
            return g.out_degrees[a.second] < g.out_degrees[b.second];
        }
        return a.first < b.first;
    });
    for (int i = 0; i < g.V; ++i) {
        this->order[enhanced_indegrees[i].second] = i;
    }
    for (int i = 0; i < g.V; ++i) {
        std::cout << i << ":" << this->order[i] << " ";
    }
}

void co::State::perturbate(co::Sampler &sampler) {
    int i = sampler.sample_vertex();
    int j = sampler.sample_vertex();
    while (i == j) {
        j = sampler.sample_vertex();
    }
    std::swap(this->order[i], this->order[j]);
}

void co::State::evaluate_full(co::DGraph &g) {
    this->value = 0;
    // sum over all outgoing edges for every vertex
    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            // add it to removed only when the edge goes against the order
            if (this->order[start] > this->order[end.vertex]) {
                this->value += end.cost;
            }
        }
    }
}

void co::State::save_solution(co::DGraph &g, std::string &path) {
    std::ofstream f(path, std::ios_base::out);
    f << this->value << std::endl;

    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->order[start] > this->order[end.vertex]) {
                // add 1 to vertices to shift to 1-based
                f << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;

    f.close();
}

void co::State::println(co::DGraph &g) {
    std::cout << this->value << std::endl;

    for (int start = 0; start < this->V; ++start) {
        for (co::Vertex &end : g.out_edges[start]) {
            if (this->order[start] > this->order[end.vertex]) {
                // add 1 to vertices to shift to 1-based
                std::cout << start + 1 << " " << end.vertex + 1 << std::endl;
            }
        }
    }
    std::cout << std::endl;
}
