/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Every formula (line in equation) is processed here. Read futher comments at each method.
 * 
 */

#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "literal.hpp"
#include "formula.hpp"
#include "../utils.hpp"
#include <vector>
#include <iostream>


/**
 * Formula is constructed with certain restrictions. 
 * One of the restriction is how many active inputs can be in one term (arity).
 * 
 * Every formula is vector of literals. 
 * The state is randomly initialized.
 * If active elements limit is reached then every literal is as non-active (bitwise value is stored anyways).
 * 
 * Non-zeros vector hold indices of active elements (latter very useful for fitness and circuit's area).
 */
Formula::Formula(const int term_count, const unsigned int arity, const ReferenceBits &reference_bits) {
    non_zeros.resize(term_count);
    int cnt = 0;

    for (int term = 0; term < term_count; term++) {
        for (size_t i = 0; i < reference_bits.input.size(); i++) {
            Literal l;
            if (static_cast<uint>(non_zeros[term].size()) >= arity) {
                l.state = State::IsNot;
                l.value = reference_bits.input[i];
            } else {
                l.state = State(utils::randint(0, 3));
                if (l.state == State::Not) {
                    l.value = ~reference_bits.input[i];
                    non_zeros[term].push_back(cnt);
                } else if (l.state == State::IsNot) {
                    l.value = reference_bits.input[i];
                } else {
                    l.value = reference_bits.input[i];
                    non_zeros[term].push_back(cnt);
                }
            }
            cnt++;
            literals.push_back(l);
        }
    }
}


/**
 * Fitness is overall score of matched reference bits.
 * Evaluation is done in a way from left to right - first left term, then second left term etc.
 * In the same way evaluation is done for ANDs. This is done in the same way as two-level gate evaluation.
 */
void Formula::calculate_fitness(const ReferenceBits &reference_bits, const int idx_out) {
    size_t shift = 0;
    for (size_t i = 0; i < non_zeros.size(); i++) {
        if (non_zeros[i].size() != 0) {
            break;
        }
        shift++;
    }

    auto evaluate_term = [&]() {
        Bitset term_out = literals[non_zeros[shift][0]].value;
        for (size_t i = 1; i < non_zeros[shift].size(); i++) {
            term_out &= literals[non_zeros[shift][i]].value;
        }
        return term_out;
    };
    
    if (shift == non_zeros.size()) {
        Bitset tmp(reference_bits.input[0].size(), 0);
        // tmp = tmp | reference_bits.output[idx_out]; // DNF
        tmp = tmp ^ reference_bits.output[idx_out];
        fitness = tmp.count();
        return;
    }

    Bitset out = evaluate_term();
    for (shift = shift + 1; shift < non_zeros.size(); shift++) {
        if (non_zeros[shift].size() == 0) continue;
        out = out ^ evaluate_term();
    }

    out = out ^ reference_bits.output[idx_out];
    fitness = out.count();
}

/**
 * Prints out every formula in specific style (easy for visual debugging).
 * Command line with ANSI escape codes - it can cause some troubles, hence ascii-only method.
 */
void Formula::print_circuit(const int inputs_count) {
    int cnt = 0;
    for (auto &x : literals) {
        int x_cnt = cnt % inputs_count;
        if (x_cnt == 0 && cnt != 0) std::cout << " ⊕ ";
        if (x.state == State::Is) {
            std::cout << "\033[0;32m" << "x" << x_cnt;
        } else if (x.state == State::Not) {
            std::cout << "\033[0;31m" << "x̂" <<  x_cnt;
        } else {
            std::cout << "\033[0;90m" << "x" << x_cnt;
        }
        std::cout << "\033[0m";
        cnt++;
    }
    std::cout << "\033[0m" << std::endl;
}

/**
 * Prints out every formula in ANSI free style, delimited with comma.
 * Optional switch `print-ascii true`.
 * 1 is active element.
 * -1 is negated.
 * 0 is unused.
 */
void Formula::print_circuit_ascii_only() {
    size_t cnt = 0;
    for (auto &x : literals) {
        if (x.state == State::Is) {
            std::cout << "1";
        } else if (x.state == State::Not) {
            std::cout << "-1";
        } else {
            std::cout << "0";
        }
        if (cnt < literals.size() - 1) std::cout << ",";
        cnt++;
    }
    std::cout << std::endl;
}

/**
 * Redundant method which prints each literal with index, bit value, and function (in/active).
 * Usage is debugging purpose.
 */
void Formula::print_bits(const int term_count, const int inputs_count) {
    for (int term = 0; term < term_count; term++) {
        for (int i = 0; i < inputs_count; i++) {
            int shift = i + (term * inputs_count);
            std::cout << "gen: " << term
                << " in: " << i << " fun: " << static_cast<uint>(literals[shift].state)
                << " " << literals[shift].value << std::endl;
        }
        std::cout << std::endl;
    }
}

/**
 * Mutation is done by random chosen input being legally modified.
 * 
 * Arity sets rough boundaries. Whether arity in one term is already full, thus one literal (its index) has to be deactivated, and
 * another one going to be activated (a small note - mutation can be negative, neutral, or positive).
 * 
 * Always one mutation one call. If you want to mutate 5 times you have to call this method 5 times.
 */
void Formula::mutate(const Parameters &p, const ReferenceBits &reference_bits) {
    int idx_literal = utils::randint(0, literals.size());     // index of literal that is going to be changed
    int idx_term = idx_literal / reference_bits.input.size(); // position detection of term
    State new_state = State(utils::randint(0, 3)); // new state is generated

    if (new_state == State::IsNot) {
        auto &vec = non_zeros[idx_term];
        auto it = std::find(vec.begin(), vec.end(), idx_literal);
        if (it != vec.end()) { // erase if was found in non_zeros
            vec.erase(it);
        }
        int idx_input = idx_literal % reference_bits.input.size();
        literals[idx_literal].value = reference_bits.input[idx_input];
        literals[idx_literal].state = new_state;
    }

    else if (new_state == State::Is) {
        if (non_zeros[idx_term].size() == p.arity) { // removed from nonzeros
            auto &vec = non_zeros[idx_term];
            auto it = std::find(vec.begin(), vec.end(), idx_literal);
            if (it != vec.end()) { // erase if was found in non_zeros
                vec.erase(it);
            } else {
                int idx = utils::randint(0, vec.size());
                literals[vec[idx]].state = State::IsNot;
                vec.erase(vec.begin() + idx);
            }
        }
        int idx_input = idx_literal % reference_bits.input.size();
        literals[idx_literal].state = new_state;
        literals[idx_literal].value = reference_bits.input[idx_input];
        auto &vec = non_zeros[idx_term];
        auto it = std::find(vec.begin(), vec.end(), idx_literal);
        if (it == vec.end()) {
            vec.push_back(idx_literal);
        }
    }

    else if (new_state == State::Not) {
        if (non_zeros[idx_term].size() == p.arity) { // removed from non_zeros
            auto &vec = non_zeros[idx_term];
            auto it = std::find(vec.begin(), vec.end(), idx_literal);
            if (it != vec.end()) { // erase if was found in non_zeros
                vec.erase(it);
            } else {
                int idx = utils::randint(0, vec.size());
                literals[vec[idx]].state = State::IsNot;
                vec.erase(vec.begin() + idx);
            }
        }
        int idx_input = idx_literal % reference_bits.input.size();
        literals[idx_literal].state = new_state;
        literals[idx_literal].value = ~reference_bits.input[idx_input];
        auto &vec = non_zeros[idx_term];
        auto it = std::find(vec.begin(), vec.end(), idx_literal);
        if (it == vec.end()) {
            vec.push_back(idx_literal);
        }
    }
}
