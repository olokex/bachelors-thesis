#include <iostream>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "function.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include "circuit.hpp"

void evolution(const Parameters &param, const ReferenceBits &reference_bits) {
    srand(param.seed);
    std::cout << "seed: " << param.seed << std::endl;

    std::vector<Circuit> population;
    for (int i = 0; i < param.lambda + 1; i++) {
        Circuit c(param, reference_bits);
        c.evaluate(reference_bits.input.size());
        c.calculate_fitness(reference_bits);
        population.push_back(c);
    }

    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();

    for (unsigned int gen = 0; gen < param.generations; gen++) {
        for (int i = 0; i < param.lambda; i++) {
            population[i] = fittest;
            population[i].mutate(param.mutation_rate, param.allowed_functions, reference_bits.input.size());
            population[i].evaluate(reference_bits.input.size());
            population[i].calculate_fitness(reference_bits);
            if (population[i].fitness == 0) {
                population[i].print_circuit_cgpviewer(param, reference_bits);
                std::cout << "generation: " << gen << std::endl;
                if (param.print_used_gates) {
                    population[i].print_used_gates(reference_bits.input.size(), param.allowed_functions);
                }
                return;
            }
        }
        Circuit &new_fittest = utils::get_fittest_invidiual(population);
        if (fittest.fitness >= new_fittest.fitness) {
            fittest = new_fittest;
        }
        if (gen % param.print_count == 0 && param.print_fitness) {
            std::cout << "generation: " << gen << " best fitness: " << fittest.fitness << std::endl;
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

        Parameters params(argc, argv);
        ReferenceBits reference_bits(params.path);
        evolution(params, reference_bits);
    } catch (const std::runtime_error &err) {
        std::cerr << "ERROR runtime: " << err.what() << std::endl;
        return 1;
    }
}
