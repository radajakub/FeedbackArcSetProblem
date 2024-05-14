#ifndef __ALNS_H__
#define __ALNS_H__

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <utility>

#include "builders.h"
#include "graph.h"
#include "state.h"

const double MAX_DESTROY_RATIO = 0.1;
const int RESTART_ITERS = 100;

namespace co {

    typedef std::vector<int> op_change;
    typedef std::function<op_change(DGraph &g, State &s, std::mt19937 &rng)> destroy_op;
    typedef std::function<void(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng)> repair_op;
    typedef std::pair<destroy_op, repair_op> op;

    namespace destroy {
        op_change random(DGraph &g, State &s, std::mt19937 &rng);
        op_change random_multiple(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly_multiple(DGraph &g, State &s, std::mt19937 &rng);
        op_change high_degree(DGraph &g, State &s, std::mt19937 &rng);
        op_change mostly_backwards(DGraph &g, State &s, std::mt19937 &rng);
        op_change more_incoming(DGraph &g, State &s, std::mt19937 &rng);
    };

    namespace repair {
        void random(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng);
        void greedy(DGraph &g, State &s, op_change destroyed, std::mt19937 &rng);
    };

    namespace select {

        class Selector {
        protected:
            std::mt19937 rng;
            std::uniform_int_distribution<int> a_dist;

            int t;

            Selector(int actions, std::mt19937 &rng) : rng(rng), a_dist(0, actions - 1), t(0) {}

        public:
            virtual int select() { return a_dist(rng); };
            virtual void update(int action, int reward) {};
        };

        class Random : public Selector {
        public:
            Random(int actions, std::mt19937 &rng) : Selector(actions, rng){};

            int select() override;
            void update(int action, int reward) override;
        };

        class EpsGreedy : public Selector {
        private:
            std::uniform_real_distribution<double> p_dist;
            double eps;

            std::vector<double> mu_r;

        public:
            EpsGreedy(int actions, double eps, std::mt19937 &rng) : Selector(actions, rng), eps(eps), mu_r(actions, 0), p_dist(0, 1){};

            int select() override;
            void update(int action, int reward) override;
        };

    };

    class ALNS {
    private:
        std::mt19937 rng;
        std::uniform_real_distribution<double> p_dist;
        std::chrono::steady_clock::time_point deadline;

        std::vector<op> operators;

        std::unique_ptr<co::select::Selector> selector;

        int iter;
        std::chrono::milliseconds prev_iter;
        std::chrono::steady_clock::time_point iter_start_time;

    public:
        ALNS(int seed, std::chrono::steady_clock::time_point deadline);

        State solve(DGraph &g);

        void iter_start();
        void iter_stop();

        bool can_continue();
    };
};

#endif
