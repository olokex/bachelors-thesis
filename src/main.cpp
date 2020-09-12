#include <iostream>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include "my_exception.hpp"
#include "reference_bits.hpp"
#include "utils.hpp"
#include "function.hpp"
#include "parameters.hpp"

class Cell {
public:
    int input1; //unsigned short int input1;
    int input2; //unsigned short int input2;
    Function function;
    Bitset output;
};

class Circuit {
private:
    std::vector<Cell> cells;
    std::vector<int> output_indices;

    void push_inputs(ReferenceBits reference_bits) {
        for (auto input : reference_bits.input) {
            Cell c;
            c.function = Function::In;
            c.output = input;
            cells.push_back(c);
        }
    }

public:
    unsigned int fitness = UINT_MAX;

    Circuit(Parameters param, ReferenceBits reference_bits) {
        push_inputs(reference_bits);
        auto random_input = [&](int start_cells_size, int col) {
            return utils::randint(
                col < param.level_back ? 0 : start_cells_size - param.level_back * param.row,
                start_cells_size);
        };
        #if 0
        auto random_input = [&](int start_cells_size, int col) {
            int start_bound;
            int end_bound;
            auto moved_past_inputs = [&]() {
                return col > param.level_back;
            };
            auto calc_with_inputs = [&]() {
                start_bound = 0;
                end_bound = reference_bits.input.size() + (param.level_back - 1) * param.row;
                if (end_bound > start_cells_size)
                    end_bound = start_cells_size;
            };
            auto calc_without_inputs = [&]() {
                end_bound = start_cells_size;
                start_bound = end_bound - param.level_back * param.row;
            };
            if (moved_past_inputs())
                calc_without_inputs();
            else
                calc_with_inputs();
            int chosen_input = utils::randint(start_bound, end_bound);
            return chosen_input;
        };
        #endif

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

        for (int i = 0; i < reference_bits.output.size(); i++) {
            output_indices.push_back(random_input(cells.size(), param.column + 1));
        }
        

    }

    void print_circuit_cgpviewer(Parameters param, ReferenceBits reference_bits) {
        printf("{%d,%d,%d,%d,2,1,%d}",
            reference_bits.input.size(),
            reference_bits.output.size(),
            param.column, param.row,
            param.allowed_functions.size()
        );
        for (int i = reference_bits.input.size(); i < cells.size(); i++) {
            printf("([%d]%d,%d,%d)", i, cells[i].input1, cells[i].input2, static_cast<int>(cells[i].function));
        }

        if (output_indices.size() == 1) {
            printf("(%d)", output_indices[0]);
        } else {
            printf("(%d", output_indices[0]);
            for (int i = 1; i < output_indices.size(); i++) {
                printf(",%d", output_indices[i]);
            }
            printf(")");
        }
        printf("\n\n");
    }

    void evaluate(ReferenceBits reference_bits) {
        for (auto it = cells.begin() + reference_bits.input.size(); it != cells.end(); it++) {
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

    void calculate_fitness(ReferenceBits reference_bits) {
        fitness = 0;
        for (int i = 0; i < output_indices.size(); i++) {
            fitness += (reference_bits.output[i] ^ cells[output_indices[i]].output).count();
        }
    }

    void mutate(Parameters param, ReferenceBits reference_bits) {
        int inputs_size = reference_bits.input.size();
        int cells_size = cells.size();

        for (int i = 0; i < param.mutation_rate; i++) {
            int rnd_idx = utils::randint(inputs_size, cells_size + output_indices.size());

            if (rnd_idx >= cells_size) {
                output_indices[rnd_idx - cells_size] = utils::randint(inputs_size, cells_size);
            } else {
                int allela = utils::randint(0, 3);
                switch (allela) {
                    case 0:
                        cells[rnd_idx].function = param.allowed_functions[rand() % param.allowed_functions.size()];
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

    void print_bits(ReferenceBits reference_bits) {
        for (int i = 0; i < reference_bits.input.size(); i++) {
            std::cout << "input " << i << ": " << reference_bits.input[i] << std::endl;
        }
        for (int i = reference_bits.input.size(); i < cells.size(); i++) {
            std::cout << "component " << i << ": " << cells[i].output << std::endl;
        }
    }

private:
};


Circuit &get_fittest_invidiual(std::vector<Circuit> &population) {
    Circuit *best = &population.at(0);

    for (int i = 1; i < population.size(); i++) {
        if (population[i].fitness <= best->fitness) {
            best = &population[i];
        }
    }

    return *best;
}


void evolution(Parameters &param, ReferenceBits &reference_bits) {
    srand(param.seed);
    std::cout << "seed: " << param.seed << std::endl;

    std::vector<Circuit> population;
    for (int i = 0; i < param.lambda + 1; i++) {
        Circuit c(param, reference_bits);
        c.evaluate(reference_bits);
        c.calculate_fitness(reference_bits);
        population.push_back(c);
    }

    Circuit &fittest = get_fittest_invidiual(population);
    population.pop_back();

    for (int gen = 0; gen < param.generations; gen++) {
        Circuit &fittest = get_fittest_invidiual(population);
        for (int i = 0; i < param.lambda; i++) {
            population[i] = fittest;
            population[i].mutate(param, reference_bits);
            population[i].evaluate(reference_bits);
            population[i].calculate_fitness(reference_bits);
            if (population[i].fitness == 0) {
                population[i].print_circuit_cgpviewer(param, reference_bits);
                std::cout << "fitness: " << population[i].fitness << std::endl;
                std::cout << "generation: " << gen << std::endl;
                return;
            }
        }
    }

    std::cout << "NOT FOUND" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        //std::string path("/home/olok/cgp_64bit/data/multiplier2x2.txt");
        std::string path("/home/olok/cgp_64bit/data/adder5_5.txt");
        //std::string path("/home/olok/cgp_64bit/data/parity5.txt");
        //ReferenceBits reference_bits = parse_file(path);
        if (argc == 2 && std::string(argv[1]) == "-h") {
            utils::print_help();
            return 0;
        }

        ReferenceBits reference_bits(path);
        Parameters params(argc, argv);
        evolution(params, reference_bits);
        //Circuit c(params, reference_bits);
        //c.print_circuit_cgpviewer(params, reference_bits);

    } catch (const CGPException &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }

}
