#include <vector>
#include <iostream>
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "term.hpp"
#include "cell.hpp"
#include "circuit.hpp"


Circuit::Circuit(const int literal_count, const ReferenceBits &reference_bits) {
    for (auto &out : reference_bits.output) {
        Term term(literal_count, reference_bits);
        terms.push_back(term);
    }
}

void Circuit::mutate(const int mutation, const ReferenceBits &reference_bits) {
    for (auto &c : terms) {
        c.mutate(mutation, reference_bits);
    }
}

void Circuit::print_circuit(const int inputs_count) {
    for (int i = 0; i < terms.size(); i++) {
        std::cout << "y" << i << " = ";
        terms[i].print_circuit(inputs_count);
        //terms[i].print_circuit_ascii_only(inputs_count);
    }
}

void Circuit::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits) {
    uint tmp_fit = 0;
    for (int i = 0; i < terms.size(); i++) {
        terms[i].calculate_fitness(param, reference_bits, i);
        tmp_fit += terms[i].fitness;
    }
    fitness = tmp_fit;
}