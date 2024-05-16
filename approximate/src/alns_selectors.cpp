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

int co::select::SegmentedRouletteWheel::select() {
    double p = this->p_dist(rng);
    double cum_p = 0;
    for (int i = 0; i < this->probabilities.size(); ++i) {
        cum_p += this->probabilities[i];
        if (p < cum_p) {
            return i;
        }
    }
    return this->probabilities.size() - 1;
}

void co::select::SegmentedRouletteWheel::update(int action, int reward) {
    this->scores[action] = this->theta * this->scores[action] + (1 - this->theta) * reward;
    this->segment += 1;

    if (this->segment == this->segment_size) {
        this->segment = 0;

        double sum = 0;
        for (int i = 0; i < this->scores.size(); ++i) {
            sum += this->scores[i];
        }

        for (int i = 0; i < this->scores.size(); ++i) {
            this->probabilities[i] = this->scores[i] / sum;
        }
    }
}

void co::select::SegmentedRouletteWheel::reset() {
    this->scores = std::vector<double>(this->scores.size(), 0);
    this->probabilities = std::vector<double>(this->probabilities.size(), static_cast<double>(1) / this->probabilities.size());
    this->segment = 0;
}
