#include "utils.h"

co::Sampler::Sampler() {
    std::random_device rd;
    this->rng = std::mt19937(rd());
    this->dist = std::uniform_real_distribution<double>(0, 1);
}

co::Sampler::Sampler(int seed) {
    this->rng.seed(seed);
    this->dist = std::uniform_real_distribution<double>(0, 1);
}

double co::Sampler::sample_prob() {
    return this->dist(this->rng);
}

co::Timer::Timer(int limit, int safety_margin) {
    this->limit = limit * 1000 - safety_margin;
    this->start_time = std::chrono::steady_clock::now();
}

bool co::Timer::should_stop() {
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->start_time).count();
    return duration >= this->limit;
}

int co::Timer::elapsed_time() {
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->start_time).count();
    return duration;
}

int co::argmax(std::vector<int> &vec) {
    int max_val = -1;
    int max_idx = -1;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > max_val) {
            max_val = vec[i];
            max_idx = i;
        }
    }
    return max_idx;
}

std::vector<std::pair<int, int>> co::index_vector(std::vector<int> &vector) {
    std::vector<std::pair<int, int>> indexed(vector.size());
    for (int i = 0; i < vector.size(); ++i) {
        indexed[i] = std::make_pair(vector[i], i);
    }
    return indexed;
}

int co::get_idx(std::vector<int> &vec, int val) {
    std::vector<int>::iterator it = std::find(vec.begin(), vec.end(), val);
    return std::distance(vec.begin(), it);
}

void co::print_vector(std::string &name, std::vector<int> &vec) {
    std::cout << name << ": ";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}
