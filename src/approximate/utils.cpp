#include "utils.h"

co::Sampler::Sampler(int seed) {
    this->rng.seed(seed);
    this->dist = std::uniform_real_distribution<double>(0, 1);
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
