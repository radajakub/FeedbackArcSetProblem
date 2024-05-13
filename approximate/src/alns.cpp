#include "alns.h"

co::op_change co::destroy::random(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    s.remove_vertex(v, g);

    std::stringstream ss;
    ss << "Destroyed vertex " << v << " ->";
    s.print(ss.str());

    return {v};
}

void co::repair::random(DGraph &g, State &s, co::op_change &destroyed, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);

    for (int v : destroyed) {
        int pos = v_dist(rng);
        s.place_vertex(v, pos, g);

        std::stringstream ss;
        ss << "Placed vertex " << v << " at " << pos << " -> ";
        s.print(ss.str());
    }
}

co::ALNS::ALNS(int seed, std::chrono::steady_clock::time_point deadline) {
    if (seed == -1) {
        std::random_device rd;
        this->rng = std::mt19937(rd());
    } else {
        this->rng = std::mt19937(seed);
    }

    this->deadline = deadline;
    // this->p_dist = std::uniform_real_distribution<double>(0, 1);
    this->operators = {
        std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random, co::repair::random),
        // std::make_pair<co::destroy_op, co::repair_op>(co::destroy::random, co::repair::greedy),
        // std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::random),
        // std::make_pair<co::destroy_op, co::repair_op>(co::destroy::most_costly, co::repair::greedy),
    };

    // intitilize bandit which will select the operation pair
    // this->selector = std::unique_ptr<co::select::Selector>(new co::select::EpsGreedy(this->operators.size(), 0.1, this->rng));
}

co::State co::ALNS::solve(co::DGraph &g) {
    co::State best = co::builders::out_cost(g);
    best.evaluate_full(g);

    co::State current = best;
    co::State s = best;
    while (std::chrono::steady_clock::now() < this->deadline) {
        // copy current state so it can be modified
        s = current;
        s.print("Current state: ");

        // select operation pair
        // int op_idx = this->selector->select();
        int op_idx = 0;
        std::cout << "Selected operator pair: " << op_idx << std::endl;

        co::destroy_op destroy = this->operators[op_idx].first;
        co::repair_op repair = this->operators[op_idx].second;

        // destroy current state
        co::op_change destroyed = destroy(g, s, this->rng);
        // repair the state
        repair(g, s, destroyed, this->rng);

        std::cout << s.cost << " vs ";
        s.evaluate_full(g);
        std::cout << s.cost << std::endl;

        // check if best solution was found
        if (s.cost < best.cost) {
            best = s;
            std::cout << "New best solution found: " << best.cost << std::endl;
        }

        // accept the state
        // todo: something better than greedy
        if (s.cost < current.cost) {
            current = s;
        }

        // update the bandits values
        // this->selector->update(op_idx, current.cost - s.cost);
    }

    return best;
}
