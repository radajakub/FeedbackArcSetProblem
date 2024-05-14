#include "alns.h"

co::op_change co::destroy::random(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);
    int v = v_dist(rng);

    s.remove_vertex(v, g);

    return {v};
}

co::op_change co::destroy::random_multiple(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> k_dist(2, (int)(MAX_DESTROY_RATIO * g.V));
    int k = k_dist(rng);

    std::vector<int> perm(g.V);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    std::vector<int> destroyed(k);
    for (int i = 0; i < k; ++i) {
        destroyed[i] = perm[i];
        s.remove_vertex(perm[i], g);
    }

    return destroyed;
}

co::op_change co::destroy::most_costly(DGraph &g, State &s, std::mt19937 &rng) {
    int max_cost = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int cost = 0;

        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[u.vertex] != -1 && s.ordering[v] > s.ordering[u.vertex]) {
                cost += u.cost;
            }
        }

        for (co::Vertex &u : g.in_edges[v]) {
            if (s.ordering[u.vertex] != -1 && s.ordering[v] < s.ordering[u.vertex]) {
                cost += u.cost;
            }
        }

        if (cost > max_cost) {
            max_cost = cost;
            max_vertex = v;
        }
    }

    s.remove_vertex(max_vertex, g);

    return {max_vertex};
}

co::op_change co::destroy::most_costly_multiple(DGraph &g, State &s, std::mt19937 &rng) {
    std::uniform_int_distribution<int> k_dist(2, (int)(MAX_DESTROY_RATIO * g.V));
    int k = k_dist(rng);

    std::vector<int> costs(g.V, 0);

    // get costs of every vertex
    for (co::Edge &e : g.edges) {
        if (s.ordering[e.source] > s.ordering[e.target]) {
            costs[e.source] += e.cost;
        }
    }

    // argsort them
    std::vector<std::pair<int, int>> indexed_costs = co::enumerate(costs);
    std::sort(indexed_costs.begin(), indexed_costs.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first > b.first;
    });

    std::vector<int> destroyed(k);
    for (int i = 0; i < k; ++i) {
        destroyed[i] = indexed_costs[i].second;
        s.remove_vertex(indexed_costs[i].second, g);
    }

    return destroyed;
}

co::op_change co::destroy::high_degree(DGraph &g, State &s, std::mt19937 &rng) {
    int max_degree = -1;
    int max_vertex = -1;

    for (int v = 0; v < g.V; ++v) {
        int degree = g.out_degrees[v] + g.in_degrees[v];
        if (degree > max_degree) {
            max_degree = degree;
            max_vertex = v;
        }
    }

    s.remove_vertex(max_vertex, g);

    return {max_vertex};
}

co::op_change co::destroy::mostly_backwards(DGraph &g, State &s, std::mt19937 &rng) {
    // compute backward cost and forward cost
    std::vector<int> forward_cost(g.V, 0);
    std::vector<int> backward_cost(g.V, 0);

    for (int v = 0; v < g.V; ++v) {
        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                forward_cost[v] += u.cost;
            } else {
                backward_cost[v] += u.cost;
            }
        }
    }

    // remove all which have higher cost backward than forward
    std::vector<int> destroyed;
    for (int i = 0; i < g.V; ++i) {
        if (backward_cost[i] > forward_cost[i]) {
            s.remove_vertex(i, g);
            destroyed.push_back(i);
        }
    }
    return destroyed;
}

co::op_change co::destroy::more_incoming(DGraph &g, State &s, std::mt19937 &rng) {
    // compute backward cost and forward cost
    std::vector<int> income_cost(g.V, 0);
    std::vector<int> outgoing_gain(g.V, 0);

    for (int v = 0; v < g.V; ++v) {
        for (co::Vertex &u : g.out_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                outgoing_gain[v] += u.cost;
            }
        }
        for (co::Vertex &u : g.in_edges[v]) {
            if (s.ordering[v] < s.ordering[u.vertex]) {
                income_cost[v] += u.cost;
            }
        }
    }

    // remove all which have higher cost backward than forward
    std::vector<int> destroyed;
    for (int i = 0; i < g.V; ++i) {
        if (income_cost[i] > outgoing_gain[i]) {
            s.remove_vertex(i, g);
            destroyed.push_back(i);
        }
    }
    return destroyed;
}

void co::repair::random(DGraph &g, State &s, co::op_change destroyed, std::mt19937 &rng) {
    std::uniform_int_distribution<int> v_dist(0, g.V - 1);

    for (int v : destroyed) {
        int pos = v_dist(rng);
        s.place_vertex(v, pos, g);
    }
}

void co::repair::greedy(DGraph &g, State &s, co::op_change destroyed, std::mt19937 &rng) {
    // shuffle the destroyed vertices to provide randomness
    if (destroyed.size() > 1)
        std::shuffle(destroyed.begin(), destroyed.end(), rng);

    // for every destroyed vertex find the best position
    for (int v : destroyed) {
        int best_pos = -1;
        int best_cost = std::numeric_limits<int>::max();
        for (int pos = 0; pos < g.V; ++pos) {
            int cost = s.cost;
            for (co::Vertex &u : g.out_edges[v]) {
                if (s.ordering[u.vertex] != -1 && pos > s.ordering[u.vertex]) {
                    cost += u.cost;
                }
            }

            for (co::Vertex &u : g.in_edges[v]) {
                if (s.ordering[u.vertex] != -1 && pos <= s.ordering[u.vertex]) {
                    cost += u.cost;
                }
            }

            if (cost < best_cost) {
                best_pos = pos;
                best_cost = cost;
            }
        }
        s.place_vertex(v, best_pos, g);
    }
}

int co::select::Random::select() {
    return a_dist(rng);
}

void co::select::Random::update(int action, int reward) {}

int co::select::EpsGreedy::select() {
    double p = p_dist(rng);
    // with p < eps explore randomly
    // with p >= eps exploit the best action
    if (p < this->eps) {
        return a_dist(rng);
    } else {
        return std::distance(mu_r.begin(), std::max_element(mu_r.begin(), mu_r.end()));
    }
}

void co::select::EpsGreedy::update(int action, int reward) {
    ++this->t;
    // incremental update of the mean
    this->mu_r[action] = (this->mu_r[action] * (this->t - 1) + reward) / this->t;
}

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
    this->selector = std::unique_ptr<co::select::Selector>(new co::select::EpsGreedy(this->operators.size(), 0.3, this->rng));
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
