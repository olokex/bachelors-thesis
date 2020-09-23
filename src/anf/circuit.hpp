#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "literal.hpp"

class Circuit {
public:
    uint fitness = UINT_MAX;

    Circuit(const int literal_count, const ReferenceBits &reference_bits);
    void print_circuit(const int inputs_count, const bool print_ascii);
    void calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits);
    
    void print_used_gates(const int subliteral_count, const int inputs_count);

    void mutate_overall(const int mutation, const ReferenceBits &reference_bits);
    void mutate_in_row(const int mutate, const ReferenceBits &reference_bits);
    void mutate_uniform(const ReferenceBits &reference_bits, const int uniform_mutate);
    void crossover(const Circuit &c);

private:
    std::vector<Literal> literals;
};


#endif /* CIRCUIT_H */