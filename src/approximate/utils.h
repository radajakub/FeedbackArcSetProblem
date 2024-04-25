#ifndef __UTILS_H__
#define __UTILS_H__

#include <chrono>
#include <iostream>
#include <numeric>
#include <random>

namespace co {
    class Sampler {
    public:
        std::mt19937 rng;
        std::uniform_real_distribution<double> dist;
        std::uniform_int_distribution<int> vertex_dist;

        Sampler(int V, int seed);

        // sample random number from the uniform distribution [0, V-1]
        int sample_vertex();

        // sample random number from the uniform distribution [start, end]
        int sample_vertex(int start, int end);

        // sample random number from the uniform distribution [0, 1]
        double sample_prob();
    };

    class Timer {
    public:
        // limit in ms
        int limit;
        // safety margin in ms
        int safety_margin;

        std::chrono::time_point<std::chrono::steady_clock> start_time;

        Timer(int limit, int safety_margin);

        // returns bool if the time is up for terminating
        bool should_stop();

        // returns the elapsed time in ms
        int elapsed_time();
    };

    int argmax(std::vector<int> &vec);

    void print_vector(std::string &name, std::vector<int> &vec);
};

#endif
