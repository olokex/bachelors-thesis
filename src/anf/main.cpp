#include <iostream>
#include <vector>
#include "parameters.hpp"
#include "state.hpp"
#include "literal.hpp"
#include "circuit.hpp"
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include <chrono>


void evolution(const Parameters &p, const ReferenceBits &ref) {
    srand(p.seed);
    std::cout << "seed: " << p.seed << std::endl;
    std::vector<Circuit> population;

    for (int i = 0; i < p.lambda + 1; i++) {
        Circuit c(p, ref);
        c.calculate_fitness(p, ref);
        // std::cout << "fitness: " << c.fitness << std::endl;
        // c.print_circuit(ref.input.size(), false);
        // return;
        population.push_back(c);
    }

    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();
    // std::cout << population.size() << std::endl;
    for (unsigned int gen = 0; gen < p.generations; gen++) {
        for (int i = 0; i < p.lambda; i++) {
            population[i] = fittest;
            population[i].mutate_overall(p, ref);
            population[i].calculate_fitness(p, ref);
            if (population[i].fitness == 0) {
                population[i].print_circuit(ref.input.size(), p.print_ascii);
                std::cout << "generation: " << gen << std::endl;
                if (p.print_used_gates) {
                    population[i].print_used_gates(ref.input.size());
                }
                if (p.print_used_area) {
                    population[i].print_used_area(ref.input.size());
                }
                return;
            }
        }
        Circuit &new_fittest = utils::get_fittest_invidiual(population);
        if (new_fittest.fitness <= fittest.fitness) {
            fittest = new_fittest;
        }
        if (gen % p.print_count == 0 && p.print_fitness) {
            std::cout << "generation: " << gen << " best fitness: " << fittest.fitness << std::endl;
            //fittest.print_circuit(ref.input.size(), false);
        }
        for (int i = 0; i < 3; i++) {
            int idx1 = utils::randint(0, p.lambda);
            int idx2 = utils::randint(0, p.lambda);
            Circuit c = Circuit::crossover(population[idx1], population[idx2]);
            c.calculate_fitness(p, ref);
            if (c.fitness <= fittest.fitness) {
                fittest = c;
            }
        }
    }
    std::cout << "NOT FOUND" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        if (argc == 2) {
            std::string opt(argv[1]);
            if (opt == "-h" || opt == "--help") {
                Parameters::print_help();
                return 0;
            }
        }
        Parameters p(argc, argv);
        ReferenceBits ref(p.path);
        p.is_valid(ref);

        // srand(p.seed);
        // std::cout << "seed: " << p.seed << std::endl;

        // Circuit c(p, ref);
        // c.print_circuit(ref.input.size(), p.print_ascii);
        // c.calculate_fitness(p, ref);
        // std::cout << "fitness: " << c.fitness << std::endl;
        // std::cout << "================" << std::endl;
        // c.mutate_overall(p, ref);
        // c.print_circuit(ref.input.size(), p.print_ascii);
        // c.calculate_fitness(p, ref);
        // std::cout << "fitness: " << c.fitness << std::endl;
        // c.mutate_overall(p, ref);
        // std::cout << "================" << std::endl;
        // c.print_circuit(ref.input.size(), false);

        // Circuit c1(p, ref);
        // Circuit c2(p, ref);
        // c1.print_circuit(ref.input.size(), false);
        // c2.print_circuit(ref.input.size(), false);
        // Circuit c = Circuit::crossover(c1, c2);
        // c.mutate_overall(p, ref);
        // c.print_circuit(ref.input.size(), false);
        //c.calculate_fitness(p, ref);

        auto start = std::chrono::steady_clock::now();
        evolution(p, ref);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "time: " << elapsed_time.count() << std::endl;
    } catch (const std::runtime_error &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }
}