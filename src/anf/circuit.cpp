#include <vector>
#include <iostream>
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "parameters.hpp"
#include "formula.hpp"
#include "circuit.hpp"

Circuit::Circuit(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (size_t i = 0; i < reference_bits.output.size(); i++) {
        Formula formula(parameters.term_count, parameters.arity, reference_bits);
        formulas.push_back(formula);
    }
}

void Circuit::mutate_overall(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (int i = 0; i < parameters.mutation; i++) {
        int formula_idx = utils::randint(0, formulas.size());
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
void Circuit::crossover(const Circuit &c) {
    for (size_t i = 0; i < formulas.size(); i++) {
        if (utils::randint(0, 2) == 0) {
            formulas[i] = c.formulas[i];
        }
    }
}

void Circuit::print_used_gates(const int inputs_count) {
    int gate_and_count = 0;
    int gate_not_count = 0;
    int gates_unused = 0;
    int gate_xor_count = 0;
    for (auto &f : formulas) {
        f.used_gates_count(inputs_count);
        gate_and_count += f.gate_and_count;
        gate_xor_count += f.gate_xor_count;
        gate_not_count += f.gate_not_count;
        gates_unused += f.gates_unused;
    }
    int sum = gate_and_count + gate_not_count + gate_xor_count;
    std::cout << "used gates: " << sum << std::endl;
    std::cout << "UNused gates: " << gates_unused << std::endl;
    std::cout << "and: " << gate_and_count << std::endl;
    std::cout << "xor: " << gate_xor_count << std::endl;
    std::cout << "not: " << gate_not_count << std::endl;
}

void Circuit::print_circuit(const int inputs_count, const bool print_ascii) {
    for (size_t i = 0; i < formulas.size(); i++) {
        std::cout << "y" << i << " = ";
        if (print_ascii) {
            formulas[i].print_circuit_ascii_only(inputs_count);
        } else {
            formulas[i].print_circuit(inputs_count);
        }
    }
}

void Circuit::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits) {
    uint tmp_fit = 0;
    for (size_t i = 0; i < formulas.size(); i++) {
        formulas[i].calculate_fitness(param.term_count, reference_bits, i);
        tmp_fit += formulas[i].fitness;
    }
    fitness = tmp_fit;
}