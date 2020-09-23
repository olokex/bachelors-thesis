#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include "literal.hpp"
#include "../utils.hpp"
#include <vector>
#include <iostream>

Literal::Literal(const int literal_count, const ReferenceBits &reference_bits) {
    for (int y = 0; y < literal_count; y++) {
        for (int i = 0; i < reference_bits.input.size(); i++) {
            Cell c;
            c.state = State(utils::randint(0, 3));
            if (c.state == State::Not) {
                c.value = ~reference_bits.input[i];
            } else {
                c.value = reference_bits.input[i];
            }
            cells.push_back(c);
        }
    }
}

void Literal::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits, const int idx_out) {
    unsigned int inputs_count = reference_bits.input.size();
    unsigned int bits_count = reference_bits.input[0].size();
    Bitset out = evaluate_subliteral(bits_count, inputs_count, 0);
    for (int i = 1; i < param.literal_count; i++) {
        out = out ^ evaluate_subliteral(bits_count, inputs_count, inputs_count * i);
    }
    out = out ^ reference_bits.output[idx_out];
    fitness = out.count();
}

void Literal::print_circuit(const int inputs_count) {
    int cnt = 0;
    for (auto &x : cells) {
        int x_cnt = cnt % inputs_count;
        if (x_cnt == 0 && cnt != 0) std::cout << " ⊕ ";
        if (x.state == State::Is) {
            std::cout << "\033[0;32m" << "x" << x_cnt; // subscript
        } else if (x.state == State::Not) {
            std::cout << "\033[0;31m" << "x̂" <<  x_cnt; // subscript
        } else {
            std::cout << "\033[0;90m" << "x" << x_cnt;
        }
        std::cout << "\033[0m";
        cnt++;
    }
    std::cout << "\033[0m" << std::endl;
}

void Literal::print_circuit_ascii_only(const int inputs_count) {
    int cnt = 0;
    std::cout << "[";
    for (auto &x : cells) {
        int x_cnt = cnt % inputs_count;
        if (x_cnt == 0 && cnt != 0) std::cout << "] o [";
        if (x.state == State::Is) {
            std::cout << "1";
        } else if (x.state == State::Not) {
            std::cout << "-1";
        } else {
            std::cout << "0";
        }
        if (x_cnt != 4) std::cout << ",";
        cnt++;
    }
    std::cout << "]";
    std::cout << std::endl;
}

void Literal::print_bits(const int literal_count, const int inputs_count) {
    for (int i = 0; i < literal_count; i++) {
        for (int y = 0; y < inputs_count; y++) {
            int shift = y + (i * inputs_count);
            std::cout << "gen: " << i
                << " in: " << y << " fun: " << static_cast<uint>(cells[shift].state)
                << " " << cells[shift].value << std::endl;
        }
        std::cout << std::endl;
    }
}

void Literal::mutate(const ReferenceBits &reference_bits) {
    int idx_cell = utils::randint(0, cells.size());
    State new_state = State(utils::randint(0, 3));
    if (cells[idx_cell].state == new_state) return;
    
    int input = idx_cell % reference_bits.input.size();
    if (new_state == State::Is) {
        cells[idx_cell].value = reference_bits.input[input];
    } else if (new_state == State::Not) {
        cells[idx_cell].value = ~reference_bits.input[input];
    }
    cells[idx_cell].state = new_state;
}

void Literal::uniform_mutation(const ReferenceBits &reference_bits, const int chance) {
    for (int i = 0; i < cells.size(); i++) {
        if (utils::randint(0, 100) < chance) {
            State new_state = State(utils::randint(0, 3));
            if (cells[i].state == new_state) continue;

            int input = i % reference_bits.input.size();
            if (new_state == State::Is) {
                cells[i].value = reference_bits.input[input];
            } else if (new_state == State::Not) {
                cells[i].value = ~reference_bits.input[input];
            }
            cells[i].state = new_state;
        }
    }
}


Bitset Literal::evaluate_subliteral(const int bits_count, const int input_count, const int start) {
    Bitset out(bits_count, 0);
    int shift = 0;
    for (; shift < input_count; shift++) {
        if (cells[start + shift].state != State::IsNot) {
            out = cells[start + shift].value;
            break;
        }
    }
    for (int i = shift + 1; i < input_count; i++) {
        if (cells[start + i].state != State::IsNot) {
            out &= cells[start + i].value;
        }
    }
    return out;
}

int Literal::count_state(const int subliteral, const int inputs_count, State s) {
    return std::count_if(cells.begin() + (subliteral * inputs_count),
                        cells.begin() + (subliteral * inputs_count) + inputs_count,
                        [&](Cell c){return c.state == s;});
}

void Literal::used_gates_count(const int inputs_count) {
    gate_xor_count = (cells.size() / inputs_count) - 1;
    for (int subliteral = 0; subliteral < (cells.size() / inputs_count); subliteral++) {
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