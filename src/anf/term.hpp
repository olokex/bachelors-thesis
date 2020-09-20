#ifndef TERM_H
#define TERM_H
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include <vector>

class Term {
public:
    unsigned int fitness = UINT_MAX;

    Term(const int literal_count, const ReferenceBits &reference_bits);
    void calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits, const int idx_out);
    void print_circuit(const int inputs_count);
    void print_circuit_ascii_only(const int inputs_count);
    void print_bits(const int literal_count, const int inputs_count);
    void mutate(const int mutation, const ReferenceBits &reference_bits);

private:
    std::vector<Cell> cells;
    Bitset evaluate_subterm(const int bits_count, const int input_count, const int start);
};


#endif /* TERM_H */