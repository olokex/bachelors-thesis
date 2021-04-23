/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Circuit is a class that is moreless wrapper that provides interconnection among formulas.
 *      Methods are writen with intetion for simple use, and easier access to desired functionality.
 * 
 */

#include <vector>
#include <iostream>
#include <set>
#include "parameters.hpp"
#include "formula.hpp"
#include "circuit.hpp"
#include "../reference_bits.hpp"
#include "../utils.hpp"
#include "../process_size.hpp"

/**
 * Circuit constructor which calls formula(s) constructor(s). Recommended to check out the formula class instead. 
 */
Circuit::Circuit(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (size_t i = 0; i < reference_bits.output.size(); i++) {
        Formula formula(parameters.term_count, parameters.arity, reference_bits);
        formulas.push_back(formula);
    }
}

/**
 * Mutates N times value in a formula that is chosen randomly.
 */
void Circuit::mutate_overall(const Parameters &parameters, const ReferenceBits &reference_bits) {
    for (int i = 0; i < parameters.mutation; i++) {
        int formula_idx = utils::randint(0, formulas.size());
        formulas[formula_idx].mutate(parameters, reference_bits);
    }
}

/**
 * Method returns tuple of used gates (xor, and, not). Those gates are count with optimization on mind.
 * The "not" gate(s) is actually count only once ("inverted out of a circuit beforehand").
 *
 * The "and" gate(s) reduction is provided as index-term pattern(s).
 * For each formula is checked the pattern of term (idx of input) and inserted in set, an instance:
 *      012, 01, 13, 134, ...
 *      if enconters same pattern (same indices) 01 and 01 the "and" gate is count only once.
 *      Subpatterns are not incount -> only of entire term's indices.
 * 
 * The "xor" gate(s) are count from "non-zeros" vector which hold current literals in vectors.
 *      This means if the vector has only 5 elements, there will be 4 XORs as the output.
 */ 
std::tuple<int, int, int> Circuit::used_gates_optimized(const int inputs_count) {
    std::set<std::vector<int>> used_elements;
    std::set<int> nots;
    int xor_count = 0;
    int and_count = 0;

    for (auto &f: formulas) {
        xor_count += f.non_zeros.size() - 1; // XORs in one formula
        for (auto &nz : f.non_zeros) {
            std::sort(nz.begin(), nz.end());
            if (nz.size() == 0) xor_count--;
            std::vector<int> pattern;
            
            for (auto &l : nz) {
                int idx = l % inputs_count;
                if (f.literals[l].state == State::Not) {
                    nots.insert(idx);
                }
                pattern.emplace_back(idx);
            }

            if (used_elements.find(pattern) == used_elements.end() && pattern.size() > 1) {  
                used_elements.insert(pattern);
                and_count += pattern.size() - 1;
            }
        }
    }
    return {xor_count, and_count, nots.size()};
}


/**
 * This method is very similar to optimized option beside it doesn't operate the optimization with pattern sets.
 * This was found as better for area optimization during EA run. Higher redundance seems to have a positive impact.
 */
std::tuple<int, int, int> Circuit::used_gates(const int inputs_count) {
    std::set<int> nots;
    int xor_count = 0;
    int and_count = 0;

    for (auto &f: formulas) {
        xor_count += f.non_zeros.size() - 1; // XORs in one formula
        for (auto &nz : f.non_zeros) {
            if (nz.size() == 0) xor_count--;
            
            for (auto &l : nz) {
                int idx = l % inputs_count;
                if (f.literals[l].state == State::Not) {
                    nots.insert(idx);
                }
            }
            
            if (nz.size() > 1) {
                and_count += nz.size() - 1;
            }
        }
    }
    return {xor_count, and_count, nots.size()};
}

/**
 * Output of this method is used gates. Depends on the parameter "optimized".
 * If the optimization is required it will print out used gates with optimized pattern matching,
 * otherwise it will print out redundant gates in terms/formulas.
 */
void Circuit::print_used_gates(const int inputs_count, bool optimized) {
    auto [xor_count, and_count, not_count] = (optimized ? used_gates_optimized(inputs_count) : used_gates(inputs_count));
    int sum = and_count + not_count + xor_count;
    std::cout << "used gates: " << sum << std::endl;
    std::cout << "and: " << and_count << std::endl;
    std::cout << "xor: " << xor_count << std::endl;
    std::cout << "not: " << not_count << std::endl;
}

/**
 * Method calculates used are on the chip in 45 nm process.
 */
void Circuit::calculate_used_area(const int inputs_count, bool optimized) {
    auto [xor_count, and_count, not_count] = (optimized ? used_gates_optimized(inputs_count) : used_gates(inputs_count));
    area = and_count * gate_size::And + xor_count * gate_size::Xor + not_count * gate_size::Not;
}

/**
 * Prints out found circuit, dependent on the optional parameter in case console does not support non-ascii characters.
 */
void Circuit::print_circuit(const int inputs_count, const bool print_ascii) {
    for (size_t i = 0; i < formulas.size(); i++) {
        if (print_ascii) {
            formulas[i].print_circuit_ascii_only();
        } else {
            formulas[i].print_circuit(inputs_count);
        }
    }
}

/**
 * Calls each formula with request of its fitness.
 */
void Circuit::calculate_fitness(const ReferenceBits &reference_bits) {
    uint tmp_fit = 0;
    for (size_t i = 0; i < formulas.size(); i++) {
        formulas[i].calculate_fitness(reference_bits, i);
        tmp_fit += formulas[i].fitness;
    }
    fitness = tmp_fit;
}

/**
 * Crossover for two formulas. Commented section is for one-point crossover for all formulas involved in a circuit.
 *     formula yn | term - term - term | term contains indices of active inputs.
 * 
 * parent 1 => y0 | 01  - 23 - 134 |
 *             y1 | 123 - 01 - 24  |
 * 
 * parent 2 => y0 | 02  - 13 - 134 |
 *             y1 | 024 - 01 - 34  |
 
 * (point first dash for y0 and y1)
 * offspring => y0 | 01  - 13 - 134 |
 *              y1 | 123 - 01 - 34  |
 * 
 * The uncommented section is for each formula within random generated point of crossover.
 * parent 1 => y0 | 01  - 23 - 134 |
 *             y1 | 123 - 01 - 24  |
 * 
 * parent 2 => y0 | 02  - 13 - 134 |
 *             y1 | 024 - 01 - 34  |
 *
 * (point first dash for y0)
 * (point second dash for y1)
 * offspring => y0 | 01  - 23 - 134 |
 *              y1 | 123 - 01 - 34  |
 */
Circuit Circuit::crossover(Circuit parent1, Circuit parent2) {
    Circuit offspring = parent1;

    // auto size = offspring.formulas[0].literals.size();
    // // std::cout << "literals: " << size << std::endl;
    // auto nonzerossize = offspring.formulas[0].non_zeros.size();
    // // std::cout << "non_zeros/terms: " << nonzerossize << std::endl;
    // int inputs_count = size / nonzerossize;
    // int cross_point = utils::randint(1, nonzerossize);
    // int shift = cross_point * inputs_count;

    for (size_t i = 0; i < offspring.formulas.size(); i++) {
        auto size = offspring.formulas[i].literals.size();
        auto nonzerossize = offspring.formulas[i].non_zeros.size();
        int inputs_count = size / nonzerossize;
        int cross_point = utils::randint(1, nonzerossize);
        int shift = cross_point * inputs_count;
        
        for (size_t lit = shift; lit < size; lit++) {
            offspring.formulas[i].literals[lit] = parent2.formulas[i].literals[lit];
        }
        for (size_t idx = cross_point; idx < nonzerossize; idx++) {
            offspring.formulas[i].non_zeros[idx] = parent2.formulas[i].non_zeros[idx];
        }
    }
    return offspring;
}