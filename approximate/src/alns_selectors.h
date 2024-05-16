#ifndef __ALNS_SELECTORS_H__
#define __ALNS_SELECTORS_H__

#include <iostream>
#include <random>
#include <vector>

#include "utils.h"

namespace co {
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
            virtual void reset() {};
        };

        class Random : public Selector {
        public:
            Random(int actions, std::mt19937 &rng) : Selector(actions, rng){};

            int select() override;
            void update(int action, int reward) override;
            void reset() override;
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
            void reset() override;
        };

        class UCB : public Selector {
        private:
            double alpha;
            std::vector<double> mu;
            std::vector<int> n;
            std::vector<double> ucb;

        public:
            UCB(int actions, double alpha, std::mt19937 &rng) : Selector(actions, rng), alpha(alpha), mu(actions, 0), n(actions, 0), ucb(actions, std::numeric_limits<double>::max()){};

            int select() override;
            void update(int action, int reward) override;
            void reset() override;
        };

    };
};

#endif
