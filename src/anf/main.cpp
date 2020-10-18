#include <iostream>
#include <vector>
#include "parameters.hpp"
#include "state.hpp"
#include "literal.hpp"
#include "circuit.hpp"
#include "../reference_bits.hpp"
#include "../utils.hpp"

void evolution(const Parameters &p, const ReferenceBits &ref) {
    srand(p.seed);
    std::cout << "seed: " << p.seed << std::endl;
    std::vector<Circuit> population;

    for (int i = 0; i < p.lambda + 1; i++) {
        Circuit c(p, ref);
        population.push_back(c);
    }
    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();

    for (unsigned int gen = 0; gen < p.generations; gen++) {
        for (int i = 0; i < p.lambda; i++) {
            population[i] = fittest;
            //population[i].mutate_in_row(p.mutation, ref);
            //population[i].mutate_uniform(ref, p.mutation);
            
            //population[i].mutate_in_row(p.mutation_row, ref);
            //population[i].mutate_uniform(ref, p.uniform_mutation);
            //population[i].crossover(fittest);
            population[i].mutate_overall(p, ref);
            population[i].calculate_fitness(p, ref);
            if (population[i].fitness == 0) {
                population[i].print_circuit(ref.input.size(), p.print_ascii);
                std::cout << "generation: " << gen << std::endl;
                if (p.print_used_gates) {
                    population[i].print_used_gates(ref.input.size());
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
        }
    }
    std::cout << "NOT FOUND" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        Parameters p(argc, argv);
        ReferenceBits ref(p.path);
        p.is_valid(ref);        evolution(p, ref);
    } catch (const std::runtime_error &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }
}