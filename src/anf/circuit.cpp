#include <vector>
#include <iostream>
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "parameters.hpp"
#include "literal.hpp"
#include "cell.hpp"
#include "circuit.hpp"


Circuit::Circuit(const int literal_count, const ReferenceBits &reference_bits) {
    for (auto &out : reference_bits.output) {
        Literal literal(literal_count, reference_bits);
        literals.push_back(literal);
    }
}

void Circuit::mutate_overall(const int mutation, const ReferenceBits &reference_bits) {
    for (int i = 0; i < mutation; i++) {
        int literal_idx = utils::randint(0, literals.size());
        literals[literal_idx].mutate(reference_bits);
    }
}

void Circuit::mutate_in_row(const int mutate, const ReferenceBits &reference_bits) {
    for (auto &literal : literals) {
        for (int i = 0; i < mutate; i++) {
            literal.mutate(reference_bits);
        }
    }
}

void Circuit::mutate_uniform(const ReferenceBits &reference_bits, const int mutate) {
    for (auto &l : literals) {
        l.uniform_mutation(reference_bits, mutate);
    }
}

void Circuit::crossover(const Circuit &c) {
    for (int i = 0; i < literals.size(); i++) {
        if (utils::randint(0, 2) == 0) {
            literals[i] = c.literals[i];
        }
    }
}

void Circuit::print_used_gates(const int subliteral_count, const int inputs_count) {
    int gate_and_count = 0;
    int gate_not_count = 0;
    int gates_unused = 0;
    int gate_xor_count = 0;
    for (auto &l : literals) {
        l.used_gates_count(inputs_count);
        gate_and_count += l.gate_and_count;
        gate_xor_count += l.gate_xor_count;
        gate_not_count += l.gate_not_count;
        gates_unused += l.gates_unused;
    }
    int sum = gate_and_count + gate_not_count + gate_xor_count;
    std::cout << "used gates: " << sum << std::endl;
    std::cout << "UNused gates: " << gates_unused << std::endl;
    std::cout << "and: " << gate_and_count << std::endl;
    std::cout << "xor: " << gate_xor_count << std::endl;
    std::cout << "not: " << gate_not_count << std::endl;
}

void Circuit::print_circuit(const int inputs_count, const bool print_ascii) {
    for (int i = 0; i < literals.size(); i++) {
        std::cout << "y" << i << " = ";
        if (print_ascii) {
            literals[i].print_circuit_ascii_only(inputs_count);
        } else {
            literals[i].print_circuit(inputs_count);
        }
    }
}

void Circuit::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits) {
    uint tmp_fit = 0;
    for (int i = 0; i < literals.size(); i++) {
        literals[i].calculate_fitness(param.literal_count, reference_bits, i);
        tmp_fit += literals[i].fitness;
    }
    fitness = tmp_fit;
}