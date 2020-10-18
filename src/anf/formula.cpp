#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "literal.hpp"
#include "formula.hpp"
#include "../utils.hpp"
#include <vector>
#include <iostream>

Formula::Formula(const int term_count, const int arity, const ReferenceBits &reference_bits) {
    non_zeros.resize(term_count);
    for (int term = 0; term < term_count; term++) {
        for (size_t i = 0; i < reference_bits.input.size(); i++) {
            Literal l;
            if (non_zeros[term].size() >= arity) {
                l.state = State::IsNot;
                l.value = reference_bits.input[i];
            } else {
                l.state = State(utils::randint(0, 3));
                if (l.state == State::Not) {
                    l.value = ~reference_bits.input[i];
                    non_zeros[term].push_back(i + term * term_count);
                } else if (l.state == State::IsNot) {
                    l.value = reference_bits.input[i];
                } else {
                    l.value = reference_bits.input[i];
                    non_zeros[term].push_back(i + term * term_count);
                }
            }
            literals.push_back(l);
        }
    }
}

void Formula::calculate_fitness(const int term_count, const ReferenceBits &reference_bits, const int idx_out) {
    unsigned int inputs_count = reference_bits.input.size();
    unsigned int bits_count = reference_bits.input[0].size();
    Bitset out = evaluate_term(bits_count, inputs_count, 0);
    for (int i = 1; i < term_count; i++) {
        out = out ^ evaluate_term(bits_count, inputs_count, inputs_count * i);
    }
    out = out ^ reference_bits.output[idx_out];
    fitness = out.count();
}

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

void Formula::print_circuit_ascii_only(const int inputs_count) {
    int cnt = 0;
    std::cout << "[";
    for (auto &x : literals) {
        int x_cnt = cnt % inputs_count;
        if (x_cnt == 0 && cnt != 0) std::cout << "] o [";
        if (x.state == State::Is) {
            std::cout << "1";
        } else if (x.state == State::Not) {
            std::cout << "-1";
        } else {
            std::cout << "0";
        }
        if (x_cnt != inputs_count - 1) std::cout << ",";
        cnt++;
    }
    std::cout << "]";
    std::cout << std::endl;
}

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

void Formula::mutate(const Parameters &p, const ReferenceBits &reference_bits) {
    int idx_cell = utils::randint(0, literals.size());
    
    int term_position = idx_cell / p.term_count;
    if (non_zeros[term_position].size() >= p.arity) {
        int i = utils::randint(0, non_zeros[term_position].size());
        
        literals[non_zeros[term_position][i]].state = State::IsNot;
        non_zeros[term_position].erase(non_zeros[term_position].begin() + i);
    }

    State new_state = State(utils::randint(0, 3));
    int input = idx_cell % reference_bits.input.size();
    if (new_state == State::Is) {
        literals[idx_cell].value = reference_bits.input[input];
        non_zeros[term_position].push_back(idx_cell);
    } else if (new_state == State::Not) {
        literals[idx_cell].value = ~reference_bits.input[input];
        non_zeros[term_position].push_back(idx_cell);
    }
    literals[idx_cell].state = new_state;
}

void Formula::uniform_mutation(const ReferenceBits &reference_bits, const int chance) {
    for (size_t i = 0; i < literals.size(); i++) {
        if (utils::randint(0, 100) < chance) {
            State new_state = State(utils::randint(0, 3));
            if (literals[i].state == new_state) continue;

            int input = i % reference_bits.input.size();
            if (new_state == State::Is) {
                literals[i].value = reference_bits.input[input];
            } else if (new_state == State::Not) {
                literals[i].value = ~reference_bits.input[input];
            }
            literals[i].state = new_state;
        }
    }
}

Bitset Formula::evaluate_term(const int bits_count, const int input_count, const int start) {
    Bitset out(bits_count, 0);
    int shift = 0;
    for (; shift < input_count; shift++) {
        if (literals[start + shift].state != State::IsNot) {
            out = literals[start + shift].value;
            break;
        }
    }
    for (int i = shift + 1; i < input_count; i++) {
        if (literals[start + i].state != State::IsNot) {
            out &= literals[start + i].value;
        }
    }
    return out;
}

int Formula::count_state(const int subliteral, const int inputs_count, State s) {
    return std::count_if(literals.begin() + (subliteral * inputs_count),
                        literals.begin() + (subliteral * inputs_count) + inputs_count,
                        [&](Literal c){return c.state == s;});
}

void Formula::used_gates_count(const int inputs_count) {
    gate_xor_count = (literals.size() / inputs_count) - 1;
    for (size_t subliteral = 0; subliteral < (literals.size() / inputs_count); subliteral++) {
        int not_count = count_state(subliteral, inputs_count, State::Not);
        int is_count = count_state(subliteral, inputs_count, State::Is);
        
        if ((is_count + not_count) > 1) {
            gate_and_count += (is_count + not_count) - 1;
        }
        if (is_count == 0 && not_count == 0) {
            gate_xor_count--;
        }
        gate_not_count += not_count;
        gates_unused += inputs_count - (not_count + is_count);
    }
}