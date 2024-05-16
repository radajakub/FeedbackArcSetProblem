#include "alns_selectors.h"

int co::select::Random::select() {
    return a_dist(rng);
}

void co::select::Random::update(int action, int reward) {}
void co::select::Random::reset() {}

int co::select::EpsGreedy::select() {
    double p = p_dist(rng);
    // with p < eps explore randomly
    // with p >= eps exploit the best action
    if (p < this->eps) {
        return a_dist(rng);
    }
    return co::argmax(this->mu_r);
}
void co::select::EpsGreedy::reset() {
    this->mu_r = std::vector<double>(this->mu_r.size(), 0);
    this->t = 0;
}

void co::select::EpsGreedy::update(int action, int reward) {
    ++this->t;
    // incremental update of the mean
    this->mu_r[action] = (this->mu_r[action] * (this->t - 1) + reward) / this->t;
}

int co::select::UCB::select() {
    // select the action with the highest UCB value
    return co::argmax(this->ucb);
}

void co::select::UCB::update(int action, int reward) {
    ++this->t;
    ++this->n[action];
    this->mu[action] = (this->mu[action] * (this->t - 1) + reward) / this->t;
    this->ucb[action] = this->mu[action] + std::sqrt((this->alpha * std::log(this->t)) / (2 * this->n[action]));
}

void co::select::UCB::reset() {
    this->mu = std::vector<double>(this->mu.size(), 0);
    this->n = std::vector<int>(this->n.size(), 0);
    this->ucb = std::vector<double>(this->ucb.size(), 0);
    this->t = 0;
}
