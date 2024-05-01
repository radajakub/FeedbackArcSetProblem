#include "ea.h"

co::State co::ea::find_bsf(std::vector<co::State> &population) {
    int best_index = -1;
    int best_value = std::numeric_limits<int>::max();
    for (int i = 0; i < population.size(); ++i) {
        co::State &s = population[i];
        if (s.cost < best_value) {
            best_index = i;
            best_value = s.cost;
        }
    }
    return population[best_index];
}

std::vector<co::State> co::ea::init(DGraph &g, int n, Sampler &sampler) {
    std::vector<co::State> population;
    population.reserve(n);

    // push all build heuristics
    population.push_back(co::builders::out_degree(g));
    population.push_back(co::builders::in_degree(g));
    population.push_back(co::builders::out_cost(g));
    population.push_back(co::builders::in_cost(g));

    // fill with randomly sampled
    for (int i = 4; i < n; ++i) {
        population.push_back(co::builders::random(g, sampler));
    }

    for (co::State &s : population) {
        s.evaluate_full(g);
        s.print_ordering();
    }

    return population;
}

std::vector<int> co::ea::select(std::vector<co::State> &population, int k, int tournamen_size, co::Sampler &sampler) {
    std::vector<int> indices(k);

    int winner_idx;
    int competitor_idx;
    int best_value;

    for (int i = 0; i < k; ++i) {
        winner_idx = -1;
        best_value = std::numeric_limits<int>::max();

        std::vector<int> competitors = sampler.sample_population(tournamen_size, population.size());

        for (int cidx : competitors) {
            co::State &competitor = population[cidx];
            if (competitor.cost < best_value) {
                winner_idx = cidx;
                best_value = competitor.cost;
            }
        }

        indices[i] = winner_idx;
    }

    return indices;
}

std::pair<co::State, co::State> co::ea::pmx_cross(DGraph &g, State &p1, State &p2, Sampler &sampler) {
    // select two points of crossover
    std::pair<int, int> points = sampler.sample_vertex_pair();

    int start = points.first;
    int end = points.second;
    // std::cout << "segment: " << start << " - " << end << std::endl;

    // create a child
    co::State pc1(g.V);
    co::State pc2(g.V);

    // create mappings
    std::vector<int> mapping1(g.V, -1);
    std::vector<int> mapping2(g.V, -1);

    for (int i = start; i <= end; ++i) {
        pc1.ordering[i] = p2.ordering[i];
        mapping1[p2.ordering[i]] = p1.ordering[i];
        pc2.ordering[i] = p1.ordering[i];
        mapping2[p1.ordering[i]] = p2.ordering[i];
    }

    int e;
    for (int i = 0; i < start; ++i) {
        e = p1.ordering[i];
        while (mapping1[e] != -1) {
            e = mapping1[e];
        }
        pc1.ordering[i] = e;
        mapping1[e] = p1.ordering[i];

        e = p2.ordering[i];
        while (mapping2[e] != -1) {
            e = mapping2[e];
        }
        pc2.ordering[i] = e;
        mapping2[e] = p2.ordering[i];
    }

    for (int i = end + 1; i < g.V; ++i) {
        e = p1.ordering[i];
        while (mapping1[e] != -1) {
            e = mapping1[e];
        }
        pc1.ordering[i] = e;
        mapping1[e] = p1.ordering[i];

        e = p2.ordering[i];
        while (mapping2[e] != -1) {
            e = mapping2[e];
        }
        pc2.ordering[i] = e;
        mapping2[e] = p2.ordering[i];
    }

    return std::make_pair(pc1, pc2);
}

co::State co::ea::evolve(DGraph &g, EvolutionConfiguration &config, co::Sampler &sampler, co::Timer &timer) {
    // initalize
    std::vector<co::State> population = co::ea::init(g, config.population_size, sampler);

    co::State bsf = co::ea::find_bsf(population);

    // evolve while there is time
    int iter = 0;
    while (!timer.should_stop()) {
        ++iter;
        // select states for breeding
        std::vector<int> parents = co::ea::select(population, config.select_size, config.tournament_size, sampler);

        // breed all pairs
        std::vector<co::State> children;
        children.reserve(config.select_size * (config.select_size - 1));

        for (int i = 0; i < config.select_size; ++i) {
            for (int j = i + 1; j < config.select_size; ++j) {
                std::pair<co::State, co::State> children_pair = co::ea::pmx_cross(g, population[parents[i]], population[parents[j]], sampler);

                co::State &c1 = children_pair.first;
                c1.evaluate_full(g);
                children.push_back(c1);
                if (sampler.sample_prob() < config.mutation_probability) {
                    co::State mutated = co::modifiers::perturbate(g, c1, sampler);
                    children.push_back(mutated);
                }

                co::State &c2 = children_pair.second;
                c2.evaluate_full(g);
                children.push_back(c2);
                if (sampler.sample_prob() < config.mutation_probability) {
                    co::State mutated = co::modifiers::perturbate(g, c2, sampler);
                    children.push_back(mutated);
                }
            }
        }

        co::State new_bsf = co::ea::find_bsf(children);
        if (new_bsf.cost < bsf.cost) {
            bsf = new_bsf;
            std::cout << "New bsf: " << bsf.cost << std::endl;
        }

        std::vector<co::State> family;
        family.reserve(population.size() + children.size());
        family.insert(family.end(), population.begin(), population.end());
        family.insert(family.end(), children.begin(), children.end());

        std::vector<int> new_population = co::ea::select(family, config.population_size, config.tournament_size, sampler);
        population.clear();
        population.reserve(config.population_size);
        for (int idx : new_population) {
            population.push_back(family[idx]);
        }
    }

    std::cout << "Iterations: " << iter << std::endl;

    return bsf;
}
