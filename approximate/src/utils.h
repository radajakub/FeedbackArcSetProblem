#ifndef __UTILS_H__
#define __UTILS_H__

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>

namespace co {
    class Sampler {
    public:
        std::mt19937 rng;
        std::uniform_real_distribution<double> dist;

        Sampler();
        Sampler(int seed);

        // sample random number from the uniform distribution [0, 1]
        double sample_prob();
    };

    class Timer {
    public:
        // limit in ms
        int limit;

        std::chrono::time_point<std::chrono::steady_clock> start_time;

        Timer(int limit, int safety_margin);

        // returns bool if the time is up for terminating
        bool should_stop();

        // returns the elapsed time in ms
        int elapsed_time();
    };

    int argmax(std::vector<int> &vec);

    // wrap every item with an index as the second item of the tuple
    std::vector<std::pair<int, int>> index_vector(std::vector<int> &vector);

    int get_idx(std::vector<int> &vec, int val);

    void print_vector(std::string &name, std::vector<int> &vec);

    int sample_v(int V, Sampler &sampler);
    std::pair<int, int> sample_v_pair(int V, Sampler &sampler);
};

#endif
