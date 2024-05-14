#include "alns_selectors.h"

int co::select::Random::select() {
    return a_dist(rng);
}

void co::select::Random::update(int action, int reward) {}

int co::select::EpsGreedy::select() {
    double p = p_dist(rng);
    // with p < eps explore randomly
    // with p >= eps exploit the best action
    if (p < this->eps) {
        return a_dist(rng);
    } else {
        int argmax = -1;
        int max = std::numeric_limits<int>::min();

        for (int i = 0; i < this->mu_r.size(); ++i) {
            if (this->mu_r[i] > max) {
                argmax = i;
                max = this->mu_r[i];
            }
        }
        return argmax;
    }
}

void co::select::EpsGreedy::update(int action, int reward) {
    ++this->t;
    // incremental update of the mean
    this->mu_r[action] = (this->mu_r[action] * (this->t - 1) + reward) / this->t;
}

int co::select::UCB::select() {
    // select the action with the highest UCB value
    int argmax = -1;
    int max = std::numeric_limits<int>::min();

    for (int i = 0; i < this->ucb.size(); ++i) {
        if (this->ucb[i] > max) {
            argmax = i;
            max = this->ucb[i];
        }
    }
    return argmax;
}

void co::select::UCB::update(int action, int reward) {
    ++this->t;
    ++this->n[action];
    this->mu[action] = (this->mu[action] * (this->t - 1) + reward) / this->t;
    this->ucb[action] = this->mu[action] + std::sqrt((this->alpha * std::log(this->t)) / (2 * this->n[action]));
}
