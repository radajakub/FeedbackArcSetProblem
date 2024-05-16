#include "alns.h"

co::ALNS::ALNS(int seed, std::chrono::steady_clock::time_point deadline, int V) {
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
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::backward_adjacent, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::backward_adjacent, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::adjacent, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::adjacent, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_adjacent, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_adjacent, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::high_degree, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::high_degree, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::mostly_backwards, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::mostly_backwards, co::repair::greedy),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::more_incoming, co::repair::random),
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::more_incoming, co::repair::greedy),
    };

    if (V > 5) {
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_multiple, co::repair::random));
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_multiple, co::repair::greedy));
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_range, co::repair::random));
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random_range, co::repair::greedy));
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_multiple, co::repair::random));
        this->operators.push_back(std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly_multiple, co::repair::greedy));
    }

    this->builders = {
        co::build::bidirect_ratio,
        co::build::in_cost,
        co::build::out_cost,
        co::build::in_degree,
        co::build::out_degree,
        co::build::bidirect,
        co::build::bidirect_max,
        co::build::bidirect_min,
        co::build::bidirect_abs,
    };

    this->restart_builders = {
        co::build::random,
        co::build::bidirect_shuffle,
    };

    this->builder_dist = std::uniform_int_distribution<int>(0, this->restart_builders.size() - 1);

    // intitilize bandit which will select the operation pair
    this->selector = std::unique_ptr<co::select::Selector>(new co::select::SegmentedRouletteWheel(this->operators.size(), 0.99, 10, this->rng));
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::UCB(this->operators.size(), 2.0, this->rng));
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::EpsGreedy(this->operators.size(), 0.3, this->rng));
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::Random(this->operators.size(), this->rng));

    // this->acceptor = std::unique_ptr<co::accept::Acceptor>(new co::accept::HillClimbing());
    // this->acceptor = std::unique_ptr<co::accept::Acceptor>(new co::accept::MovingAverage(100, 0.5));
    this->acceptor = std::unique_ptr<co::accept::Acceptor>(new co::accept::SimulatedAnnealing(10, 0.01, 0.99, this->rng));

    this->iter = 0;
    this->prev_iter = std::chrono::milliseconds(0);
}

co::State co::ALNS::solve(co::DGraph &g) {
    co::State best = this->choose_builder()(g, this->rng);
    best.evaluate_full(g);
    best = co::ls::swap_neighbors(best, g, this->rng);
    if (g.V < 5) {
        best = co::ls::swap_neighbors(best, g, this->rng);
    } else {
        best = co::ls::shift_range(best, g, this->rng);
    }

    // best.check_validtity(g);

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
        // s.check_consistency(g);
        // co::check_duplicates(destroyed);

        repair(g, s, destroyed, this->rng);

        // s.check_validtity(g);

        // todo: remove before submission
        // int val = s.cost;
        // s.evaluate_full(g);
        // if (val != s.cost) {
        //     std::cout << val << " vs " << s.cost << std::endl;
        //     exit(1);
        // }

        // check if best solution was found
        int reward = 0;
        if (s.cost < best.cost) {
            reward = std::max(10, reward);
            best = s;
            // todo: remove before submission
            std::cout << "[" << this->iter << "] New best solution found: " << best.cost << std::endl;
        } else if (s.cost < current.cost) {
            reward = std::max(5, reward);
        }

        // accept the state
        if (this->acceptor->accept(current.cost, s.cost)) {
            reward = std::max(2, reward);
            current = s;
            no_change_iters = 0;
        } else {
            // restart if no improvement was done in a long time
            ++no_change_iters;
            if (no_change_iters > RESTART_ITERS) {
                no_change_iters = 0;
                current = this->choose_builder()(g, this->rng);
                // current = co::build::random(g, this->rng);
                current.evaluate_full(g);
                current = co::ls::shift_range(current, g, this->rng);
                // this->acceptor->reset();
                // this->selector->reset();
            }
        }

        // update the bandit values
        // int reward = current.cost - s.cost;
        this->selector->update(op_idx, reward);

        this->iter_stop();
    }

    return best;
}

co::builder co::ALNS::choose_builder() {
    if (this->next_builder < this->builders.size()) {
        return this->builders[this->next_builder++];
    }
    return this->restart_builders[this->builder_dist(this->rng)];
}

void co::ALNS::iter_start() {
    this->iter_start_time = std::chrono::steady_clock::now();
}

void co::ALNS::iter_stop() {
    ++this->iter;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    this->prev_iter = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->iter_start_time);
}

bool co::ALNS::can_continue() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return (now + this->prev_iter) < this->deadline;
}
