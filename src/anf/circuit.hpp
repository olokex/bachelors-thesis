#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "formula.hpp"

class Circuit {
public:
    uint fitness = UINT_MAX;

    Circuit(const Parameters &parameters, const ReferenceBits &reference_bits);
    void print_circuit(const int inputs_count, const bool print_ascii);
    void calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits);
    
    void print_used_gates(const int inputs_count);
    void print_used_area(const int inputs_count);

    void mutate_overall(const Parameters &parameters, const ReferenceBits &reference_bits);
    void mutate_in_row(const int mutate, const ReferenceBits &reference_bits);
    void mutate_uniform(const ReferenceBits &reference_bits, const int uniform_mutate);

    static Circuit crossover(Circuit parent1, Circuit parent2);

private:
    std::vector<Formula> formulas;
};

#endif /* CIRCUIT_H */