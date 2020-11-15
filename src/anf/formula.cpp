#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "literal.hpp"
#include "formula.hpp"
#include "../utils.hpp"
#include <vector>
#include <iostream>

Formula::Formula(const int term_count, const int arity, const ReferenceBits &reference_bits) {
    non_zeros.resize(term_count);
    int cnt = 0;

/*
    for (int term = 0; term < term_count; term++) {
        for (int i = 0; i < reference_bits.input.size(); i++) {
            Literal l;
            if (term == 5 && i == 4) {
                l.state = State::Is;
                l.value = reference_bits.input[i];
                non_zeros[term].push_back(cnt);
                std::cout << "tady" << std::endl;
            } else if (i == term && i != 4) {
                l.state = State::Is;
                l.value = reference_bits.input[i];
                non_zeros[term].push_back(cnt);
            } else {
                l.state = State::IsNot;
                l.value = ~reference_bits.input[i];
            }
            literals.push_back(l);
            cnt++;
        }
    }
*/


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
/*
    for (auto &t : non_zeros) {
        for (auto &x : t) {
            std::cout << x << " || ";
        }
    }
    std::cout << std::endl;
*/
}

void Formula::calculate_fitness_new(const ReferenceBits &reference_bits, const int idx_out) {
    int shift = 0;
    for (int i = 0; i < non_zeros.size(); i++) {
        if (non_zeros[i].size() != 0) {
            shift = i;
            break;
        }
    }
    //std::cout << "shift 1 :" << shift << std::endl;
    // Bitset out = literals[non_zeros[shift][0]].value;

    auto evaluate_term = [&]() {
        //std::cout << "fun: " << static_cast<int>(literals[non_zeros[shift][0]].state) << std::endl;
        Bitset term_out = literals[non_zeros[shift][0]].value;
        for (int i = 1; i < non_zeros[shift].size(); i++) {
            term_out &= literals[non_zeros[shift][i]].value;
        }
        //std::cout << "shift: " << shift << std::endl;
        return term_out;
    };
    
    Bitset out = evaluate_term();
    // Bitset out(refence_bits.input[0].size());
    // TODO theoretical segfault if whole formula is empty - isnot states => almost impossible?
    for (shift = shift + 1; shift < non_zeros.size(); shift++) {
        if (non_zeros[shift].size() == 0) continue;
        out = out ^ evaluate_term();
        //std::cout << "inside: " << i << std::endl;
    }

    // std::cout << "out bef: " << out << std::endl;
    out = out ^ reference_bits.output[idx_out];
    // std::cout << "out af: " << out << std::endl;
    fitness = out.count();
}

void Formula::calculate_fitness(const int term_count, const ReferenceBits &reference_bits, const int idx_out) {
    unsigned int inputs_count = reference_bits.input.size();
    unsigned int bits_count = reference_bits.input[0].size();
    Bitset out = evaluate_term(bits_count, inputs_count, 0);
    for (int i = 1; i < term_count; i++) {
        out = out ^ evaluate_term(bits_count, inputs_count, inputs_count * i);
    }
    // std::cout << "out bef: " << out << std::endl;
    out = out ^ reference_bits.output[idx_out];
    // std::cout << "out af: " << out << std::endl;
    fitness = out.count();
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

    for (auto &x : non_zeros) {
        for (auto &t : x) {
            std::cout << t << " || ";
        }
        std::cout << std::endl;
    }

}

void Formula::print_circuit_ascii_only(const int inputs_count) {
    int cnt = 0;
    //std::cout << "[";
    for (auto &x : literals) {
        int x_cnt = cnt % inputs_count;
        //if (x_cnt == 0 && cnt != 0) std::cout << "] o [";
        if (x.state == State::Is) {
            std::cout << "1";
        } else if (x.state == State::Not) {
            std::cout << "-1";
        } else {
            std::cout << "0";
        }
        if (cnt < literals.size() - 1) std::cout << ",";
        //if (x_cnt != inputs_count - 1) std::cout << ",";
        cnt++;
    }
    //std::cout << "]";
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
    int idx_literal = utils::randint(0, literals.size());     // vygenerování IDX literalu co se má změnit
    int idx_term = idx_literal / reference_bits.input.size(); // zjištění pozice termu

    // std::cout << "size ref" << reference_bits.input.size() << std::endl;

    // std::cout << "lit i: " << idx_literal << " term i: " << idx_term << std::endl;
    State new_state = State(utils::randint(0, 3)); // vygenerování nového stavu

    if (new_state == State::IsNot) {
        auto &vec = non_zeros[idx_term];
        auto it = std::find(vec.begin(), vec.end(), idx_literal);
        if (it != vec.end()) { // vymazani pokud se nasel v non_zeros
            vec.erase(it);
        }
        int idx_input = idx_literal % reference_bits.input.size(); // teoreticky nadbytecne
        literals[idx_literal].value = reference_bits.input[idx_input];
        literals[idx_literal].state = new_state;
    }

    if (new_state == State::Is) {
        if (non_zeros[idx_term].size() == p.arity) { // odstrani z nonzeros
            auto &vec = non_zeros[idx_term];
            auto it = std::find(vec.begin(), vec.end(), idx_literal);
            if (it != vec.end()) { // vymazani pokud se nasel v non_zeros
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

    if (new_state == State::Not) {
        if (non_zeros[idx_term].size() == p.arity) { // odstrani z nonzeros
            auto &vec = non_zeros[idx_term];
            auto it = std::find(vec.begin(), vec.end(), idx_literal);
            if (it != vec.end()) { // vymazani pokud se nasel v non_zeros
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


/*
    if (non_zeros[idx_term].size() == p.arity) {
        // std::cout << "tady " << non_zeros.size() << std::endl;
        auto &vec = non_zeros[idx_term];
        auto it = std::find(vec.begin(), vec.end(), idx_literal);
        if (it == vec.end()) {
            std::cout << "nenasel" << std::endl;
            if (vec.size() != 0) {
                int i = utils::randint(0, vec.size());
                literals[non_zeros[idx_term][i]].state = State::IsNot;
                vec.erase(vec.begin() + i);
                std::cout << "i: " << i << std::endl;
            }
        } else {
            std::cout << "nasel" << std::endl;
            // std::cout << "lit i: " << idx_literal << " term i: " << idx_term << std::endl;
            vec.erase(it);
            literals[idx_literal].state = State::IsNot;
        }
    }


    literals[idx_literal].state = new_state;
    if (new_state != State::IsNot) {
        int idx_input = idx_literal % reference_bits.input.size();
        if (new_state == State::Not) {
            std::cout << "po" << std::endl;
           literals[idx_literal].value = ~reference_bits.input[idx_input];
        } else {
            std::cout << "pred" << std::endl;
            literals[idx_literal].value = reference_bits.input[idx_input];
        }
        non_zeros[idx_term].push_back(idx_literal);
        std::cout << "pridano" << std::endl;
    }
    
    std::cout << "new state: " << static_cast<int>(new_state) << std::endl;
    
    // if (non_zeros[idx_term].size()  p.arity) {
    //     // if (utils::is_in_vector(non_zeros[idx_term], idx_literal)) {
    //     //     non_zeros[idx_term].erase(std::find(non_zeros[idx_term].begin(), non_zeros[idx_term].end(), idx_literal));
    //     // }
    //     std::cout << "tady" << std::endl;
    // }
/*
    int idx_cell = utils::randint(0, literals.size());
    //std::cout << "idx " << idx_cell << " ";
    // literals[idx_cell].state = State::Not;
    //std::cout << "term cnt: " << p.term_count << std::endl;
    int term_position = idx_cell / reference_bits.input.size();
    //std::cout << " pos : " << term_position << " ";

    if (non_zeros[term_position].size() >= p.arity) {
        int i = utils::randint(0, non_zeros[term_position].size());
        
        literals[non_zeros[term_position][i]].state = State::IsNot;
        non_zeros[term_position].erase(non_zeros[term_position].begin() + i);
    }

    State new_state = State(utils::randint(0, 3));
    int input = idx_cell % reference_bits.input.size();
    if (new_state == State::Is) {
        literals[idx_cell].value = reference_bits.input[input];
        if (!utils::is_in_vector(non_zeros[term_position], idx_cell)) {
            non_zeros[term_position].push_back(idx_cell);
        }
        literals[idx_cell].state = new_state;
    } else if (new_state == State::Not) {
        literals[idx_cell].value = ~reference_bits.input[input];
        if (!utils::is_in_vector(non_zeros[term_position], idx_cell)) {
            non_zeros[term_position].push_back(idx_cell);
        }
        literals[idx_cell].state = new_state;
    }
*/
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

int Formula::count_state(const int term, const int inputs_count, State s) {
    return std::count_if(literals.begin() + (term * inputs_count),
                        literals.begin() + (term * inputs_count) + inputs_count,
                        [&](Literal c){return c.state == s;});
}

void Formula::used_gates_count(const int inputs_count) {
    gate_xor_count = (literals.size() / inputs_count) - 1;
    for (size_t term = 0; term < (literals.size() / inputs_count); term++) {
        int not_count = count_state(term, inputs_count, State::Not);
        int is_count = count_state(term, inputs_count, State::Is);
        
        if ((is_count + not_count) > 1) {
            gate_and_count += (is_count + not_count) - 1;
        }
        if (is_count == 0 && not_count == 0) {
            gate_xor_count--;
        }
        gate_not_count += not_count;
        //gates_unused += inputs_count - (not_count + is_count);
    }
}