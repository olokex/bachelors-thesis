/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Formula's header file.
 * 
 */

#ifndef FORMULA_H
#define FORMULA_H
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "literal.hpp"
#include <vector>

class Formula {
public:
    unsigned int fitness = UINT_MAX;

    Formula(const int term_count, const unsigned int arity, const ReferenceBits &reference_bits);
    void print_circuit(const int inputs_count);
    void print_circuit_ascii_only();
    void print_bits(const int term_count, const int inputs_count);
    void mutate(const Parameters &parameters, const ReferenceBits &reference_bits);
    void calculate_fitness(const ReferenceBits &reference_bits, const int idx_out);
    std::vector<std::vector<int>> non_zeros;
    std::vector<Literal> literals;

private:
    Bitset evaluate_term(const int bits_count, const int input_count, const int start);
};


#endif /* FORMULA_H */