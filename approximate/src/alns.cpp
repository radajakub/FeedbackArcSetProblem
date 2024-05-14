#include "alns.h"

/// ########## ALNS ##########

co::ALNS::ALNS(int seed, std::chrono::steady_clock::time_point deadline) {
    if (seed == -1) {
        std::random_device rd;
        this->rng = std::mt19937(rd());
    } else {
        this->rng = std::mt19937(seed);
    }

    this->deadline = deadline;
    this->p_dist = std::uniform_real_distribution<double>(0, 1);
    this->operators = {
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_multiple, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_multiple, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_multiple, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_multiple, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::high_degree, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::high_degree, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::mostly_backwards, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::mostly_backwards, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::more_incoming, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::more_incoming, co::repair::greedy),
    };

    // intitilize bandit which will select the operation pair
    this->selector = std::unique_ptr<co::select::Selector>(new co::select::UCB(this->operators.size(), 2, this->rng));
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::EpsGreedy(this->operators.size(), 0.3, this->rng));
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::Random(this->operators.size(), this->rng));

    this->iter = 0;
    this->prev_iter = std::chrono::milliseconds(0);
}

co::State co::ALNS::solve(co::DGraph &g) {
    co::State best = co::builders::out_cost(g);
    best.evaluate_full(g);

    co::State current = best;
    co::State s = best;

    int no_change_iters = 0;

    while (this->can_continue()) {
        this->iter_start();
        // copy current state so it can be modified
        s = current;

        // select operation pair
        int op_idx = this->selector->select();

        co::destroy_op destroy = this->operators[op_idx].first;
        co::repair_op repair = this->operators[op_idx].second;

        // destroy current state
        co::op_change destroyed = destroy(g, s, this->rng);
        // repair the state
        repair(g, s, destroyed, this->rng);

        // // todo: remove before submission
        // int val = s.cost;
        // s.evaluate_full(g);
        // if (val != s.cost) {
        //     std::cout << val << " vs " << s.cost << std::endl;
        //     exit(1);
        // }

        // check if best solution was found
        if (s.cost < best.cost) {
            best = s;
            // todo: remove before submission
            std::cout << "New best solution found: " << best.cost << std::endl;
        }

        // update the bandit values
        int reward = current.cost - s.cost;
        this->selector->update(op_idx, reward);

        if (reward == 0) {
            ++no_change_iters;
            if (no_change_iters > RESTART_ITERS) {
                no_change_iters = 0;
                s = co::builders::random(g, this->rng);
                s.evaluate_full(g);
            }
        } else {
            no_change_iters = 0;
        }

        // accept the state
        // todo: something better than greedy
        double p = this->p_dist(this->rng);
        if (s.cost < current.cost && p < 0.9) {
            current = s;
        }

        this->iter_stop();
    }

    return best;
}

void co::ALNS::iter_start() {
    this->iter_start_time = std::chrono::steady_clock::now();
}

void co::ALNS::iter_stop() {
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    this->prev_iter = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->iter_start_time);
}

bool co::ALNS::can_continue() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return (now + this->prev_iter) < this->deadline;
}
