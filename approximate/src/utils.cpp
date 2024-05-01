#include "utils.h"

co::Sampler::Sampler(int V, int population_size) {
    std::random_device rd;
    this->rng = std::mt19937(rd());
    this->dist = std::uniform_real_distribution<double>(0, 1);
    this->vertex_dist = std::uniform_int_distribution<int>(0, V - 1);
    this->population_dist = std::uniform_int_distribution<int>(0, population_size - 1);
}

co::Sampler::Sampler(int V, int population_size, int seed) {
    this->rng.seed(seed);
    this->dist = std::uniform_real_distribution<double>(0, 1);
    this->vertex_dist = std::uniform_int_distribution<int>(0, V - 1);
    this->population_dist = std::uniform_int_distribution<int>(0, population_size - 1);
}

int co::Sampler::sample_vertex() {
    return this->vertex_dist(this->rng);
}

std::pair<int, int> co::Sampler::sample_vertex_pair() {
    int v1 = this->vertex_dist(this->rng);
    int v2;
    do {
        v2 = this->vertex_dist(this->rng);
    } while (v1 == v2);
    return v1 < v2 ? std::make_pair(v1, v2) : std::make_pair(v2, v1);
}

int co::Sampler::sample_vertex(int start, int end) {
    std::uniform_int_distribution<int> dist(start, end);
    return dist(this->rng);
}

int co::Sampler::sample_population() {
    return this->population_dist(this->rng);
}

int co::Sampler::sample_population(std::vector<bool> &tabu) {
    int sample;
    do {
        sample = this->population_dist(this->rng);
    } while (tabu[sample]);
    return sample;
}

double co::Sampler::sample_prob() {
    return this->dist(this->rng);
}

co::Timer::Timer(int limit, int safety_margin) {
    this->limit = limit * 1000;
    this->safety_margin = safety_margin;
    this->start_time = std::chrono::steady_clock::now();
}

bool co::Timer::should_stop() {
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->start_time).count();
    return duration + this->safety_margin >= this->limit;
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
