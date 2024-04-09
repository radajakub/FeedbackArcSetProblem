#ifndef __UTILS_H__
#define __UTILS_H__

#include <chrono>
#include <random>

namespace co {
    // remake to generator class with fixed initalizer (see chat gpt)
    class Sampler {
    public:
        std::mt19937 rng;
        std::uniform_real_distribution<double> dist;
        std::uniform_int_distribution<int> vertex_dist;

        Sampler(int V, int seed);

        int sample_vertex();

        double sample_prob();
    };

    class Timer {
    public:
        // limit in ms
        int limit;
        int safety_margin;
        std::chrono::time_point<std::chrono::steady_clock> start_time;

        // limit in s
        // safety_margin in ms
        Timer(int limit, int safety_margin);

        bool should_stop();
        int elapsed_time();
    };
};

#endif
