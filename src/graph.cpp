#include "graph.hpp"

co::DGraph::DGraph(std::vector<std::vector<int>> edges)
{
    this->E = edges.size();

    // find max V
    this->V = 0;
    for (int i = 0; i < this->E; ++i)
    {
        int v2 = edges[i][1];
        this->V = std::max(this->V, std::max(edges[i][0], edges[i][1]));
    }

    // create adjacency list [V x Ni] and cost matrix [V x V]
    this->adjacency_list = std::vector<std::vector<int>>(this->V);
    this->adjacency_matrix = std::vector<std::vector<int>>(this->V, std::vector<int>(this->V, 0));
    for (int i = 0; i < this->E; ++i)
    {
        // index from 0
        int v1 = edges[i][0] - 1;
        int v2 = edges[i][1] - 1;
        int cost = edges[i][2];
        this->adjacency_list[v1].push_back(v2);
        this->adjacency_matrix[v1][v2] = cost;
    }
}

void co::DGraph::print()
{
    std::cout << "Graph" << std::endl;
    std::cout << " - |E| = " << this->E << std::endl;
    std::cout << " - |V| = " << this->V << std::endl;
    std::cout << " - Adjacency List" << std::endl;
    for (int i = 0; i < this->V; ++i)
    {
        std::cout << " --- " << i << " -> ";
        for (int j = 0; j < this->adjacency_list[i].size(); ++j)
        {
            int v2 = this->adjacency_list[i][j];
            std::cout << v2 << "(" << this->adjacency_matrix[i][v2] << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> co::load_edges(std::string path)
{
    std::ifstream f(path, std::ios_base::in);
    int E;
    f >> E;

    std::vector<std::vector<int>> edges(E, std::vector<int>(3));

    for (int i = 0; i < E; ++i)
    {
        f >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }
    return edges;
}
