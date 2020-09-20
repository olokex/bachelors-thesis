#include "../reference_bits.hpp"
#include "parameters.hpp"
#include "cell.hpp"
#include "term.hpp"
#include "../utils.hpp"
#include <vector>
#include <iostream>

Term::Term(const int literal_count, const ReferenceBits &reference_bits) {
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

void Term::calculate_fitness(const Parameters &param, const ReferenceBits &reference_bits, const int idx_out) {
    unsigned int inputs_count = reference_bits.input.size();
    unsigned int bits_count = reference_bits.input[0].size();
    Bitset out = evaluate_subterm(bits_count, inputs_count, 0);
    for (int i = 1; i < param.literal_count; i++) {
        //std::cout << "pred " << out << std::endl;
        out = out ^ evaluate_subterm(bits_count, inputs_count, inputs_count * i);
        //std::cout << "po   " << out << std::endl;
        //std::cout << "shift range: " << reference_bits.input.size() * i << std::endl;
    }
    //out_circuit = out;
    out = out ^ reference_bits.output[idx_out]; // udelat pro vic
    //fitness = UINT_MAX;
    //int cnt = 0;
    //for (int i = 0; i < out.size(); i++) {
    //    if (out[i] != reference_bits.output[0][i]) cnt++;
    //}
    fitness = out.count();
    //fitness = static_cast<uint>(cnt);
}

void Term::print_circuit(const int inputs_count) {
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

void Term::print_circuit_ascii_only(const int inputs_count) {
    int cnt = 0;
    for (auto &x : cells) {
        int x_cnt = cnt % inputs_count;
        //if (x_cnt == 0 && cnt != 0) std::cout << " ⊕ ";
        if (x.state == State::Is) {
            std::cout << "1,"; // subscript
        } else if (x.state == State::Not) {
            std::cout << "-1,"; // subscript
        } else {
            std::cout << "0,";
        }
        //std::cout << "\033[0m";
        cnt++;
    }
    std::cout << std::endl;
}

void Term::print_bits(const int literal_count, const int inputs_count) {
    for (int i = 0; i < literal_count; i++) {
        for (int y = 0; y < inputs_count; y++) {
            int shift = y + (i * inputs_count);
            //std::cout << shift << std::endl;
            std::cout << "gen: " << i
                << " in: " << y << " fun: " << static_cast<uint>(cells[shift].state)
                << " " << cells[shift].value << std::endl;
        }
        std::cout << std::endl;
    }
}

void Term::mutate(const int mutation, const ReferenceBits &reference_bits) {
    for (int i = 0; i < mutation; i++) {
        int idx_cell = utils::randint(0, cells.size());
        State new_state = State(utils::randint(0, 3));
        if (cells[idx_cell].state == new_state) continue;
        int input = idx_cell % reference_bits.input.size();
        if (new_state == State::Is) {
            cells[idx_cell].value = reference_bits.input[input];
        } else if (new_state == State::Not) {
            cells[idx_cell].value = ~reference_bits.input[input];
        }
        cells[idx_cell].state = new_state;
        //std::cout << "mutace bunka: " << idx_cell << "state " << new_state << std::endl;
    }
}


Bitset Term::evaluate_subterm(const int bits_count, const int input_count, const int start) {
    Bitset out(bits_count, 0);
    int shift = 0;
    for (; shift < input_count; shift++) {
        if (cells[start + shift].state != State::IsNot) {
            out = cells[start + shift].value;
            break;
        }
    }
    //std::cout << shift << std::endl;
    for (int i = shift + 1; i < input_count; i++) {
        if (cells[start + i].state != State::IsNot) {
            out &= cells[start + i].value;
            //std::cout << "shift " << start + i << std::endl;
        }
    }
    //std::cout << "shift" << shift << " " << cells[start + shift].value << std::endl;
    //std::cout << "one set out - shift: " << shift << " - " << out << std::endl;
    return out;
}

