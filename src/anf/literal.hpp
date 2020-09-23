#ifndef LITERAL_H
#define LITERAL_H
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include <vector>

class Literal {
public:
    unsigned int fitness = UINT_MAX;
    int gate_and_count = 0;
    int gate_not_count = 0;
    int gates_unused = 0;
    int gate_xor_count = 0;

    Literal(const int literal_count, const ReferenceBits &reference_bits);
    void calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits, const int idx_out);
    void print_circuit(const int inputs_count);
    void print_circuit_ascii_only(const int inputs_count);
    void print_bits(const int literal_count, const int inputs_count);
    void mutate(const ReferenceBits &reference_bits);
    void used_gates_count(const int inputs_count);
    void uniform_mutation(const ReferenceBits &reference_bits, const int chance);

private:
    std::vector<Cell> cells;
    Bitset evaluate_subliteral(const int bits_count, const int input_count, const int start);
    int count_state(const int subliteral, const int inputs_count, State s);
};


#endif /* LITERAL_H */