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
        return std::distance(mu_r.begin(), std::max_element(mu_r.begin(), mu_r.end()));
    }
}

void co::select::EpsGreedy::update(int action, int reward) {
    ++this->t;
    // incremental update of the mean
    this->mu_r[action] = (this->mu_r[action] * (this->t - 1) + reward) / this->t;
}

int co::select::UCB::select() {
    // select the action with the highest UCB value
    return std::distance(ucb.begin(), std::max_element(ucb.begin(), ucb.end()));
}

void co::select::UCB::update(int action, int reward) {
    ++this->t;
    ++this->n[action];
    this->mu[action] = (this->mu[action] * (this->t - 1) + reward) / this->t;
    this->ucb[action] = this->mu[action] + std::sqrt((this->alpha * std::log(this->t)) / (2 * this->n[action]));
}
