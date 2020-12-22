#include <iostream>
#include <chrono>
#include <tuple>
#include <iomanip>
#include "parameters.hpp"
#include "state.hpp"
#include "literal.hpp"
#include "circuit.hpp"
#include "../reference_bits.hpp"
#include "../utils.hpp"


std::tuple<Circuit, uint> evolution(const Parameters &p, const ReferenceBits &ref) {
    srand(p.seed);
    std::cout << "seed: " << p.seed << std::endl;
    std::vector<Circuit> population;

    for (int i = 0; i < p.lambda + 1; i++) {
        Circuit c(p, ref);
        c.calculate_fitness(ref);
        population.push_back(c);
    }

    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();
    unsigned int gen = 0;
    for (gen = 0; gen < p.generations; gen++) {
        for (int i = 0; i < p.lambda; i++) {
            population[i] = fittest;
            population[i].mutate_overall(p, ref);
            population[i].calculate_fitness(ref);

            if (population[i].fitness == 0) {
                population[i].print_circuit(ref.input.size(), p.print_ascii);
                std::cout << "generation: " << gen << std::endl;
                if (p.print_used_gates) {
                    population[i].print_used_gates(ref.input.size());
                }
                if (p.print_used_area) {
                    population[i].calculate_used_area(ref.input.size());
                    std::cout << "area: " << population[i].area << std::endl;
                }
                return {population[i], gen};
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
    return {fittest, 0};
}

void evolution_second_criterio(const Parameters &p, const ReferenceBits &ref, std::tuple<Circuit, uint> holder) {
    std::vector<Circuit> population;
    auto [fittest, generation_remain] = holder;
    
    for (int i = 0; i < p.lambda; i++) {
        population.push_back(fittest);
    }

    std::cout << std::fixed << std::setprecision(2);
    for (uint gen = generation_remain; gen < p.generations; gen++) {
        for (int i = 0; i < p.lambda; i++) {
            population[i] = fittest;
            population[i].mutate_overall(p, ref);
            population[i].calculate_used_area(ref.input.size());
            population[i].calculate_fitness(ref);
        }

        Circuit &new_fittest = utils::get_fittest_invidiual_area(population);
        if (new_fittest.fitness <= fittest.fitness && new_fittest.area <= fittest.area) {
            fittest = new_fittest;
        }

        if (gen % p.print_count == 0 && p.print_fitness) {
            std::cout << "generation: " << gen << " area: " << fittest.area << std::endl;
        }
    }

    fittest.print_circuit(ref.input.size(), p.print_ascii);
    std::cout << "optimized area: " << fittest.area << std::endl; 
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

        auto start = std::chrono::steady_clock::now();
        std::tuple<Circuit, uint> hold = evolution(p, ref);
        if (std::get<1>(hold)) {
            evolution_second_criterio(p, ref, hold);
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "time: " << elapsed_time.count() << " ms" << std::endl;
    } catch (const std::runtime_error &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }
}