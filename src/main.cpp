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
#include "my_exception.hpp"
#include "reference_bits.hpp"
#include "utils.hpp"
#include "function.hpp"
#include "parameters.hpp"

class Cell {
public:
    unsigned short int input1;
    unsigned short int input2;
    Function function;
    boost::dynamic_bitset<> output;
};

class Circuit {
private:
    ReferenceBits &reference_bits;
    Parameters &param;

    std::vector<Cell> cells;
    std::vector<int> output_indices;

public:
    unsigned int fitness;

    Circuit(Parameters &p, ReferenceBits &ref) : reference_bits(ref), param(p) {
        for (auto input : reference_bits.input) {
            Cell c;
            c.function = Function::In;
            c.output = input;
            cells.push_back(c);
        }

        unsigned int nodes_count = param.column * param.row;
        for (int i = 0; i < reference_bits.input.size(); i++) {
            Cell c;
            int in1 = rand() % reference_bits.input.size();
            int in2 = rand() % reference_bits.input.size();
            int fun = rand() % param.allowed_functions.size();
            //std::cout << "first: " << in1;
            //std::cout << " second " << in1 << std::endl;
            c.input1 = in1;
            c.input2 = in2;
            c.function = param.allowed_functions[fun];
            cells.push_back(c);
        }

        std::cout << cells.size() << std::endl;
        int cnt = cells.size() - reference_bits.input.size();
        int loop = 0;
        for (int i = reference_bits.input.size(); i < nodes_count; i++) {
            Cell c;
            //std::cout << param.level_back << " " << param.column << std::endl; break;
            int in1 = rand() % param.column + cnt;
            int in2 = rand() % param.column + cnt;
            int fun = rand() % param.allowed_functions.size();
            //std::cout << "first: " << in1;
            //std::cout << " second " << in2 << std::endl;
            //return;
            c.input1 = in1;
            c.input2 = in2;
            c.function = param.allowed_functions[fun];
            cells.push_back(c);
            loop++;
            if (loop % param.column == 0) cnt += param.column;
        }

        for (int i = 0; i < reference_bits.output.size(); i++) {
            int out = rand() % cells.size();
            output_indices.push_back(out);
        }

    }

    void print_circuit_cgpviewer() {
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

    void evaluate() {
        //boost::dynamic_bitset<> in1, in2, out;
        Function func;
        using bitset = boost::dynamic_bitset<>;

        for (int i = reference_bits.input.size(); i < cells.size(); i++) {
            bitset &in1 = cells[cells[i].input1].output;
            bitset &in2 = cells[cells[i].input2].output;
            bitset out;
            func = cells[i].function;

            if (func == Function::In) out = in1;
            if (func == Function::Not) out = ~in1;
            if (func == Function::And) out = in1 & in2;
            if (func == Function::Or) out = in1 | in2;
            if (func == Function::Xor) out = in1 ^ in2;
            if (func == Function::Nand) out = ~(in1 & in2);
            if (func == Function::Nor) out = ~(in1 | in2);
            if (func == Function::Xnor) out = ~(in1 ^ in2);
            cells[i].output = out;
        }
    }

    void evaluate_two() {
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

    void calculate_fitness() {
        fitness = 0;
        for (int i = 0; i < output_indices.size(); i++) {
            fitness += (reference_bits.output[i] ^ cells[output_indices[i]].output).count();
        }
    }

    void mutate() {
        for (int i = 0; i < param.mutation_rate; i++) {
            // TODO drat, funkce + asi output vector???
        }
    }

    void print_bits() {
        for (int i = 0; i < reference_bits.input.size(); i++) {
            std::cout << "input " << i << ": " << reference_bits.input[i] << std::endl;
        }
        for (int i = reference_bits.input.size(); i < cells.size(); i++) {
            std::cout << "component " << i << ": " << cells[i].output << std::endl;
        }
    }

private:
};

int main(int argc, char *argv[]) {
    try {
        //std::string path("/home/olok/cgp_64bit/data/multiplier2x2.txt");
        std::string path("/home/olok/cgp_64bit/data/parity5.txt");
        //ReferenceBits reference_bits = parse_file(path);
        if (argc == 2 && std::string(argv[1]) == "-h") {
            utils::print_help();
            return 0;
        }
        ReferenceBits reference_bits(path);
        Parameters params(argc, argv);
        srand(params.seed);
        Circuit c(params, reference_bits);
        c.print_circuit_cgpviewer();
        //c.print_bits();
        //c.evaluate();
        //c.calculate_fitness();
        //c.evaluate_two();
        std::cout << c.fitness << std::endl;
    } catch (const CGPException &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }

}
