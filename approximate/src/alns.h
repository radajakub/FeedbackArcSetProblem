#ifndef __ALNS_H__
#define __ALNS_H__

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <utility>

#include "builders.h"
#include "graph.h"
#include "state.h"

const double MAX_DESTROY_RATIO = 0.1;

namespace co {

    typedef std::vector<int> op_change;
    typedef std::function<op_change(DGraph &g, State &s, std::mt19937 &rng)> destroy_op;
    typedef std::function<void(DGraph &g, State &s, op_change &destroyed, std::mt19937 &rng)> repair_op;
    typedef std::pair<destroy_op, repair_op> op;

    namespace destroy {
        op_change random(DGraph &g, State &s, std::mt19937 &rng);
        // std::vector<int> random_multiple(DGraph &g, State &s, std::mt19937 &rng);
        op_change most_costly(DGraph &g, State &s, std::mt19937 &rng);
        // std::vector<int> most_costly_multiple(DGraph &g, State &s, std::mt19937 &rng);
    };

    namespace repair {
        void random(DGraph &g, State &s, op_change &destroyed, std::mt19937 &rng);
        void greedy(DGraph &g, State &s, op_change &destroyed, std::mt19937 &rng);
    };

    namespace select {

        class Selector {
        protected:
            std::mt19937 rng;
            std::uniform_int_distribution<int> a_dist;

            Selector(int actions, std::mt19937 &rng) : rng(rng), a_dist(0, actions - 1) {}

        public:
            virtual int select() { return a_dist(rng); };
            virtual void update(int action, int reward) {};
        };

        class EpsGreedy : public Selector {
        private:
            double eps;

            std::vector<int> vals;

        public:
            EpsGreedy(int actions, double eps, std::mt19937 &rng) : Selector(actions, rng), eps(eps){};

            int select() override;
            void update(int action, int reward) override;
        };

    };

    class ALNS {
    private:
        std::mt19937 rng;
        // std::uniform_real_distribution<double> p_dist;
        std::chrono::steady_clock::time_point deadline;

        std::vector<op> operators;

        std::unique_ptr<co::select::Selector> selector;

    public:
        ALNS(int seed, std::chrono::steady_clock::time_point deadline);

        State solve(DGraph &g);
    };

    // delete operators
    // delete a random vertex (set it to -1)
    // delete a most costly vertex (with most backwards edges)
    // delete a most costly subsequence ?? or just a random subsequence

    // insert operators
    // random greedy insert on a best position
    // insert on a random position

    // eps-greedy or ucb bandit for operator selection

    // acceptance criterions for the new solution
    // only improvement
    // simulated annealing (expenential decay of probability)

    // maybe add random restarts??
};

#endif
