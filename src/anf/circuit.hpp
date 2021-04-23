/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Header file for ANF circuit class.
 * 
 */

#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include <limits>
#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "formula.hpp"

class Circuit {
public:
    uint fitness = std::numeric_limits<uint>::max();
    double area = std::numeric_limits<double>::max();

    Circuit(const Parameters &parameters, const ReferenceBits &reference_bits);
    void print_circuit(const int inputs_count, const bool print_ascii);
    void calculate_fitness(const ReferenceBits &reference_bits);
    void print_used_gates(const int inputs_count, bool optimized=false);
    void calculate_used_area(const int inputs_count, bool optimized=false);
    void mutate_overall(const Parameters &parameters, const ReferenceBits &reference_bits);

    static Circuit crossover(Circuit parent1, Circuit parent2);

private:
    std::tuple<int, int, int> used_gates_optimized(const int inputs_count);
    std::tuple<int, int, int> used_gates(const int inputs_count);
    std::vector<Formula> formulas;
};

#endif /* CIRCUIT_H */