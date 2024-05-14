#include "alns_acceptors.h"

bool co::accept::HillClimbing::accept(int current_cost, int new_cost) {
    return new_cost < current_cost;
}

bool co::accept::MovingAverage::accept(int current_cost, int new_cost) {
    // accept first k automatically
    if (k < gamma) {
        history[k++] = new_cost;
        return true;
    }

    int s_star = std::numeric_limits<int>::max();
    int sum = 0;
    for (int i = 0; i < gamma; ++i) {
        sum += history[i];
        s_star = std::min(s_star, history[i]);
    }

    double threshold = s_star + eta * (sum / gamma - s_star);

    // update the history
    history[idx++] = new_cost;
    if (idx == gamma) {
        idx = 0;
    }

    return new_cost < threshold;
}

bool co::accept::SimulatedAnnealing::accept(int current_cost, int new_cost) {
    // compute probability
    double p = std::exp((current_cost - new_cost) / T);

    // update temperature
    this->T = std::max(this->T * this->gamma, this->Tend);

    return this->dist(rng) < p;
}
