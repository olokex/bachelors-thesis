/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Main file for CGP method. Evolution second criterion as well, the core of program is done here.
 *      Method uses evolutionary strategy known as lambda + 1.
 *      Two-step multi-criteria model is used for the chip's area.
 * 
 */

#include <iostream>
#include <chrono>
#include <tuple>
#include <iomanip>
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "function.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include "circuit.hpp"

std::tuple<Circuit, uint, uint> evolution(const Parameters &param, const ReferenceBits &reference_bits) {
    srand(param.seed);
    std::cout << "seed: " << param.seed << std::endl;

    uint evaluation = 0;
    std::vector<Circuit> population;
    for (int i = 0; i < param.lambda + 1; i++) {
        Circuit c(param, reference_bits);
        c.evaluate(reference_bits.input.size());
        c.calculate_fitness(reference_bits);
        population.push_back(c);
        evaluation++;
    }

    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();

    for (unsigned int gen = 0; gen < param.generations; gen++) {
        for (int i = 0; i < param.lambda; i++) {
            population[i] = fittest;
            population[i].mutate(param.mutation_rate, param.allowed_functions, reference_bits.input.size());
            population[i].evaluate(reference_bits.input.size());
            population[i].calculate_fitness(reference_bits);
            evaluation++;
            if (population[i].fitness == 0) {
                // population[i].print_circuit_cgpviewer(param, reference_bits);
                std::cout << "evaluations: " << evaluation << std::endl;
                if (param.print_used_gates) {
                    population[i].print_used_gates(reference_bits.input.size(), param.allowed_functions);
                }
                if (param.print_used_area) {
                    population[i].calculate_used_area(reference_bits.input.size(), param.allowed_functions);
                    std::cout << "area: " << population[i].area << std::endl;
                }
                return {population[i], gen, evaluation};
            }
        }
        Circuit &new_fittest = utils::get_fittest_invidiual(population);
        if (fittest.fitness >= new_fittest.fitness) {
            fittest = new_fittest;
        }
        if (gen % param.print_count == 0 && param.print_fitness) {
            std::cout << "evaluations: " << evaluation << " best fitness: " << fittest.fitness << std::endl;
        }
    }

    std::cout << "NOT FOUND" << std::endl;
    return {fittest, 0, evaluation};
}

void evolution_second_criterio(const Parameters &param, const ReferenceBits &reference_bits, std::tuple<Circuit, uint, uint> holder) {
    std::vector<Circuit> population;
    auto [fittest, generation_remain, evaluation] = holder;
    for (int i = 0; i < param.lambda; i++) {
        population.push_back(fittest);
    }

    std::cout << std::fixed << std::setprecision(2);
    fittest.calculate_used_area(reference_bits.input.size(), param.allowed_functions);
    std::cout << "first invidual area: " << fittest.area << std::endl;
    for (uint gen = generation_remain; gen < param.generations; gen++) {
        for (int i = 0; i < param.lambda; i++) {
            population[i] = fittest;
            population[i].mutate(param.mutation_rate, param.allowed_functions, reference_bits.input.size());
            population[i].evaluate(reference_bits.input.size());
            population[i].calculate_fitness(reference_bits);
            population[i].calculate_used_area(reference_bits.input.size(), param.allowed_functions);
            evaluation++;
        }

        Circuit &new_fittest = utils::get_fittest_invidiual_area(population);
        if (new_fittest.fitness <= fittest.fitness && new_fittest.area <= fittest.area) {
            fittest = new_fittest;
        }

        if (gen % param.print_count == 0 && param.print_fitness) {
            std::cout << "evaluations: " << evaluation << " area: " << fittest.area << std::endl;
        }
    }

    // fittest.print_circuit_cgpviewer(param, reference_bits);
    if (param.print_used_gates) {
        fittest.print_used_gates(reference_bits.input.size(), param.allowed_functions);
    }
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

        Parameters params(argc, argv);
        ReferenceBits reference_bits(params.path);
        // auto start = std::chrono::steady_clock::now();
        
        std::tuple<Circuit, uint, uint> hold = evolution(params, reference_bits);
        if (std::get<1>(hold) && params.second_criterion) {
            evolution_second_criterio(params, reference_bits, hold);
        }
        
        // auto end = std::chrono::steady_clock::now();
        // auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // std::cout << "time: " << elapsed_time.count() << " ms" << std::endl;
    } catch (const std::runtime_error &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }
}
