#ifndef __UTILS_H__
#define __UTILS_H__

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
        std::uniform_int_distribution<int> vertex_dist;
        std::uniform_int_distribution<int> population_dist;

        Sampler(int V, int population_size);
        Sampler(int V, int population_size, int seed);

        // sample random number from the uniform distribution [0, V-1]
        int sample_vertex();
        std::pair<int, int> sample_vertex_pair();

        // sample random number from the uniform distribution [start, end]
        int sample_vertex(int start, int end);

        // samples an index from population size
        int sample_population();
        int sample_population(std::vector<bool> &tabu);

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

    // wrap every item with an index as the second item of the tuple
    std::vector<std::pair<int, int>> index_vector(std::vector<int> &vector);

    int get_idx(std::vector<int> &vec, int val);

    void print_vector(std::string &name, std::vector<int> &vec);
};

#endif
