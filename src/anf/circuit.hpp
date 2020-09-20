#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "term.hpp"

class Circuit {
public:
    uint fitness = UINT_MAX;

    Circuit(const int literal_count, const ReferenceBits &reference_bits);
    void mutate(const int mutation, const ReferenceBits &reference_bits);
    void print_circuit(const int inputs_count);
    void calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits);

private:
    std::vector<Term> terms;
};


#endif /* CIRCUIT_H */