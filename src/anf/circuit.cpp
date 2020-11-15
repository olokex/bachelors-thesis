#include <vector>
#include <iostream>
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "parameters.hpp"
#include "formula.hpp"
#include "circuit.hpp"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Circuit::Circuit(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (size_t i = 0; i < reference_bits.output.size(); i++) {
        Formula formula(parameters.term_count, parameters.arity, reference_bits);
        formulas.push_back(formula);
    }
}

void Circuit::mutate_overall(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (int i = 0; i < parameters.mutation; i++) {
        int formula_idx = utils::randint(0, formulas.size());
        // std::cout << "formula: " << formula_idx << std::endl;
        formulas[formula_idx].mutate(parameters, reference_bits);
    }
}

/*
void Circuit::mutate_in_row(const int mutate, const ReferenceBits &reference_bits) {
    for (auto &formula : formulas) {
        for (int i = 0; i < mutate; i++) {
            formula.mutate(reference_bits);
        }
    }
}

void Circuit::mutate_uniform(const ReferenceBits &reference_bits, const int mutate) {
    for (auto &f : formulas) {
        f.uniform_mutation(reference_bits, mutate);
    }
}
*/

void Circuit::print_used_gates(const int inputs_count) {
    int gate_and_count = 0;
    int gate_not_count = 0;
    //int gates_unused = 0;
    int gate_xor_count = 0;
    for (auto &f : formulas) {
        f.used_gates_count(inputs_count);
        gate_and_count += f.gate_and_count;
        gate_xor_count += f.gate_xor_count;
        gate_not_count += f.gate_not_count;
        //gates_unused += f.gates_unused;
    }
    int sum = gate_and_count + gate_not_count + gate_xor_count;
    std::cout << "used gates: " << sum << std::endl;
    //std::cout << "UNused gates: " << gates_unused << std::endl;
    std::cout << "and: " << gate_and_count << std::endl;
    std::cout << "xor: " << gate_xor_count << std::endl;
    std::cout << "not: " << gate_not_count << std::endl;
}

void Circuit::print_used_area(const int inputs_count) {
    int gate_and_count = 0;
    int gate_not_count = 0;
    int gate_xor_count = 0;
    for (auto &f : formulas) {
        f.used_gates_count(inputs_count);
        gate_and_count += f.gate_and_count;
        gate_xor_count += f.gate_xor_count;
        gate_not_count += f.gate_not_count;
    }
    double area = gate_and_count * 2.34 + gate_xor_count * 4.69 + gate_not_count * 1.40;
    std::cout << "area: " << area << std::endl;
}

void Circuit::print_circuit(const int inputs_count, const bool print_ascii) {
    for (size_t i = 0; i < formulas.size(); i++) {
        // std::cout << "y" << i << " = ";
        if (print_ascii) {
            formulas[i].print_circuit_ascii_only(inputs_count);
        } else {
            formulas[i].print_circuit(inputs_count);
        }
        // for (auto &l : formulas[i].non_zeros) {
        //     for (auto &t: l) {
        //         std::cout << t << " ";
        //     }
        //     std::cout << " || ";
        // }
        // std::cout << std::endl;
    }
}

void Circuit::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits) {
/*
    uint tmp_fit = 0;
    for (size_t i = 0; i < formulas.size(); i++) {
        formulas[i].calculate_fitness(param.term_count, reference_bits, i);
        tmp_fit += formulas[i].fitness;
        // std::cout << std::endl;
        // for (auto &t : formulas[i].non_zeros) {
        //     for (auto &x : t) {
        //         std::cout << x << ", ";
        //         std::cout << "s: " << static_cast<int>(formulas[i].literals[x].state) << "; ";
        //     }
        //     std::cout << std::endl;
        // }
    }
    fitness = tmp_fit;
*/

    uint tmp_fit = 0;
    for (size_t i = 0; i < formulas.size(); i++) {
        formulas[i].calculate_fitness_new(reference_bits, i);
        tmp_fit += formulas[i].fitness;
        // std::cout << "f: " << i << " fit: " << formulas[i].fitness << std::endl;
        // formulas[i].print_circuit(reference_bits.input.size());
        // for (auto &t : formulas[i].non_zeros) {
        //     for (auto &x : t) {
        //         std::cout << x << ", ";
        //     }
        //     std::cout << std::endl;
        // }
        //formulas[i].print_bits(param.term_count, reference_bits.input.size());
    }
    fitness = tmp_fit;

}



Circuit Circuit::crossover(Circuit parent1, Circuit parent2) {
    Circuit offspring = parent1;

    // auto size = offspring.formulas[0].literals.size();
    // // std::cout << "literals: " << size << std::endl;
    // auto nonzerossize = offspring.formulas[0].non_zeros.size();
    // // std::cout << "non_zeros/terms: " << nonzerossize << std::endl;
    // int inputs_count = size / nonzerossize;
    // int cross_point = utils::randint(1, nonzerossize);
    // int shift = cross_point * inputs_count;


    for (int i = 0; i < offspring.formulas.size(); i++) {
        auto size = offspring.formulas[i].literals.size();
        // std::cout << "literals: " << size << std::endl;
        auto nonzerossize = offspring.formulas[i].non_zeros.size();
        // std::cout << "non_zeros/terms: " << nonzerossize << std::endl;
        int inputs_count = size / nonzerossize;
        int cross_point = utils::randint(1, nonzerossize);

        // std::cout << "point: " << cross_point << std::endl;
        int shift = cross_point * inputs_count;
        // std::cout << "shift: " << shift << std::endl; 
        
        // for (int lit = shift; lit < size; lit++) {
        //     offspring.formulas[i].literals[lit] = parent2.formulas[i].literals[lit];
        // }
        for (int idx = cross_point; idx < nonzerossize; idx++) {
            offspring.formulas.at(i).non_zeros.at(idx) = parent2.formulas.at(i).non_zeros.at(idx);
            // offspring.formulas[i].non_zeros[idx] = parent2.formulas[i].non_zeros[idx];
        }
    }
    return offspring;
}

