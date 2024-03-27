#include "solution.h"

co::Solution::Solution(std::vector<co::Edge> &edges) {
    this->D = std::vector<co::Edge>(edges.begin(), edges.end());
    this->value = 0;
    for (co::Edge e : D) this->value += e.cost;
}

void co::Solution::save(std::string path) {
    std::ofstream f(path, std::ios_base::out);
    f << this->value << std::endl;
    // add 1 to vertices to shift to 1-based
    for (co::Edge e : this->D)
        f << e.source + 1 << " " << e.target + 1 << std::endl;
}
