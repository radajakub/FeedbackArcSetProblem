#ifndef __ALNS_ACCEPTORS_H__
#define __ALNS_ACCEPTORS_H__

#include <limits>
#include <random>
#include <vector>

namespace co {
    namespace accept {
        class Acceptor {
        private:
            int best_cost;

        public:
            virtual bool accept(int current_cost, int new_cost) { return true; };
        };

        class HillClimbing : public Acceptor {
        public:
            bool accept(int current_cost, int new_cost) override;
        };

        class MovingAverage : public Acceptor {
        private:
            int k;
            int idx;
            std::vector<int> history;
            int gamma;
            double eta;

        public:
            MovingAverage(int gamma, double eta) : k(0), idx(0), gamma(gamma), history(gamma, 0), eta(eta){};

            bool accept(int current_cost, int new_cost) override;
        };

        class SimulatedAnnealing : public Acceptor {
        private:
            double T;
            double Tend;
            double gamma;
            std::uniform_real_distribution<double> dist;
            std::mt19937 &rng;

        public:
            SimulatedAnnealing(double T, double Tend, double gamma, std::mt19937 &rng) : T(T), Tend(Tend), gamma(gamma), rng(rng), dist(0, 1){};

            bool accept(int current_cost, int new_cost) override;
        };
    };
};

#endif
