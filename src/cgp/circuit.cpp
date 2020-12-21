#include <iostream>
#include <cstdio>
#include "parameters.hpp"
#include "cell.hpp"
#include "circuit.hpp"
#include "../utils.hpp"
#include "../reference_bits.hpp"
#include "../process_size.hpp"

Circuit::Circuit(const Parameters &param, const ReferenceBits &reference_bits) {
    push_inputs(reference_bits);
    auto random_input = [&](int start_cells_size, int col) {
        return utils::randint(
            col < param.level_back ? 0 : start_cells_size - param.level_back * param.row,
            start_cells_size);
    };

    for (int col = 0; col < param.column; col++) { 
        int start_cells_size = cells.size();
        for (int row = 0; row < param.row; row++) {
            Cell c;
            c.function = param.allowed_functions[utils::randint(0, param.allowed_functions.size())];
            c.input1 = random_input(start_cells_size, col);
            c.input2 = random_input(start_cells_size, col);
            cells.push_back(c);
        }
    }

    for (unsigned int i = 0; i < reference_bits.output.size(); i++) {
        output_indices.push_back(random_input(cells.size(), param.column + 1));
    }
}

void Circuit::print_circuit_cgpviewer(const Parameters &param, const ReferenceBits &reference_bits) {
    printf("{%ld,%ld,%d,%d,2,1,%ld}",
        reference_bits.input.size(),
        reference_bits.output.size(),
        param.column, param.row,
        param.allowed_functions.size()
    );

    for (unsigned int i = reference_bits.input.size(); i < cells.size(); i++) {
        printf("([%d]%d,%d,%d)", i, cells[i].input1, cells[i].input2, static_cast<int>(cells[i].function));
    }

    std::cout << "(" << output_indices[0];
    for (unsigned int i = 1; i < output_indices.size(); i++) {
        if (i != output_indices.size()) {
            std::cout << "," << output_indices[i];
        } else {
            std::cout << output_indices[i];
        }
    }
    std::cout << ")" << std::endl;
}

void Circuit::evaluate(const int input_size) {
    for (auto it = cells.begin() + input_size; it != cells.end(); it++) {
        Bitset &in1 = cells[it->input1].output;
        Bitset &in2 = cells[it->input2].output;

        switch (it->function) {
            case Function::In: it->output = in1; break;
            case Function::Not: it->output = ~in1; break;
            case Function::And: it->output = in1 & in2; break;
            case Function::Or: it->output = in1 | in2; break;
            case Function::Xor: it->output = in1 ^ in2; break;
            case Function::Nand: it->output = ~(in1 & in2); break;
            case Function::Nor: it->output = ~(in1 | in2); break;
            case Function::Xnor: it->output = ~(in1 ^ in2); break;
        }
    }
}

void Circuit::calculate_fitness(const ReferenceBits &reference_bits) {
    fitness = 0;
    for (unsigned int i = 0; i < output_indices.size(); i++) {
        fitness += (reference_bits.output[i] ^ cells[output_indices[i]].output).count();
    }
}

void Circuit::mutate(const int mutation_rate, const std::vector<Function> &allowed_functions, const int inputs_count) {
    int cells_size = cells.size();

    for (int i = 0; i < mutation_rate; i++) {
        int rnd_idx = utils::randint(inputs_count, cells_size + output_indices.size());

        if (rnd_idx >= cells_size) {
            output_indices[rnd_idx - cells_size] = utils::randint(inputs_count, cells_size);
        } else {
            int allela = utils::randint(0, 3);
            switch (allela) {
                case 0:
                    cells[rnd_idx].function = allowed_functions[rand() % allowed_functions.size()];
                    break;
                case 1:
                    cells[rnd_idx].input1 = utils::randint(0, rnd_idx);
                    break;
                case 2:
                    cells[rnd_idx].input2 = utils::randint(0, rnd_idx);
                    break;
            }
        }
    }
}

void Circuit::print_bits(const ReferenceBits &reference_bits) {
    for (unsigned int i = 0; i < reference_bits.input.size(); i++) {
        std::cout << "input " << i << ": " << reference_bits.input[i] << std::endl;
    }
    for (unsigned int i = reference_bits.input.size(); i < cells.size(); i++) {
        std::cout << "component " << i << ": " << cells[i].output << std::endl;
    }
}

void Circuit::print_used_gates(const int inputs_count, const std::vector<Function> &allowed) {
    this->inputs_count = inputs_count;
    find_used_gates();
    find_max_delay();
    std::cout << "max delta of circuit is: " << delta << std::endl;
    std::cout << "gates used overall: " << used_gates_indices.size() << std::endl;
    for (auto &fun : allowed) {
        int cnt = 0;
        cnt = count_gates_within_function(fun);
        if (cnt > 0) {
            std::cout.width(4);
            std::cout.setf(std::ios::left);
            std::cout << function_name(fun) << " " << cnt << std::endl;
        }
    }
}

void Circuit::calculate_used_area(const int inputs_count, const std::vector<Function> &allowed) {
    this->inputs_count = inputs_count;
    find_used_gates();
    double area = 0;
    for (auto &fun : allowed) {
        int cnt = 0;
        cnt = count_gates_within_function(fun);
        switch (fun) {
            case Function::Not:
                area += cnt * gate_size::Not;
                break;
            case Function::And:
                area += cnt * gate_size::And;
                break;
            case Function::Or:
                area += cnt * gate_size::Or;
                break;
            case Function::Xor:
                area += cnt * gate_size::Xor;
                break;
            case Function::Nand:
                area += cnt * gate_size::Nand;
                break;
            case Function::Nor:
                area += cnt * gate_size::Nor;
                break;
            case Function::Xnor:
                area += cnt * gate_size::Xnor;
                break;
            case Function::In:
                break;
        }
    }
    this->area = area;
}

void Circuit::push_inputs(const ReferenceBits &reference_bits) {
    for (auto input : reference_bits.input) {
        Cell c;
        c.function = Function::In;
        c.output = input;
        cells.push_back(c);
    }
}

int Circuit::count_gates_within_function(const Function fun) {
    int cnt = 0;
    for (auto &idx : used_gates_indices) {
        if (cells[idx].function != fun) continue;
        cnt++;
    }
    return cnt;
}

bool Circuit::redundant_gate(const int idx) {
    if (cells[idx].input1 == cells[idx].input2) {
        Function fun = cells[idx].function;
        if (fun == Function::And || fun == Function::Or) {
            return true;
        }
    }
    return false;
}

void Circuit::add_pass_gate(const int idx) {
    if (idx < inputs_count) return;
    if (!utils::is_in_vector(used_gates_indices, idx)) {
        if (cells[idx].function != Function::In && !redundant_gate(idx)) {
            used_gates_indices.push_back(idx);
        }
    }
    if (cells[idx].function != Function::In && cells[idx].function != Function::Not) {
        add_pass_gate(cells[idx].input1);
        add_pass_gate(cells[idx].input2);
    } else {
        add_pass_gate(cells[idx].input1);
    }
}

void Circuit::find_used_gates() {
    for (auto &out : output_indices) {
        add_pass_gate(out);
    }
}

int Circuit::depth(const int idx) {
    if (idx < inputs_count) return 0;

    if (redundant_gate(idx)) {
        return 0 + depth(cells[idx].input1);
    }

    if (cells[idx].function == Function::In) {
        return 0 + depth(cells[idx].input1);
    }

    if (cells[idx].function == Function::Not) {
        return 1 + depth(cells[idx].input1);
    }

    int in1 = depth(cells[idx].input1);
    int in2 = depth(cells[idx].input2);

    if (in1 > in2) {
        return in1 + 1;
    } else {
        return in2 + 1;
    }
}

void Circuit::find_max_delay() {
    int max_delta = 0;
    for (auto &out : output_indices) {
        max_delta = depth(out);
        if (max_delta > delta) {
            delta = max_delta;
        }
        max_delta = 0;
    }
}