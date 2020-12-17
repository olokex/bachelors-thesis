#include "../utils.hpp"
#include "parameters.hpp"
#include <iostream>
#include <exception>
#include <stdexcept>

Parameters::Parameters() {}

Parameters::Parameters(const int argc, const char * const argv[]) {
    if (argc % 2 != 1) {
        throw std::runtime_error("Invalid parameter count");
    }

    for (int i = 1; i < argc; i += 2) {
        std::string opt(argv[i]);
        std::string val(argv[i + 1]);
        if (opt == "lambda") {
            if (!utils::parse_int(lambda, val) || lambda < 1) {
                throw std::runtime_error("Invalid value for lambda, expected number >= 1");
            }
        } else if (opt == "generations") {
            if (!utils::parse_unsigned_int(generations, val) || generations < 1) {
                throw std::runtime_error("Invalid value for generations, expected number >= 1");
            }
        } else if (opt == "print-count") {
            if (!utils::parse_int(print_count, val) || print_count < 1) {
                throw std::runtime_error("Invalid value for print-count, expected number >= 1");
            }
        } else if (opt == "path") {
            path = val; // validity provides ReferenceBits object
        } else if (opt == "print-fitness") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for print-fitness, expected true or false");
            }
            print_fitness = (val == "true");
        } else if (opt == "seed") {
            if (!utils::parse_unsigned_int(seed, val)) {
                throw std::runtime_error("Invalid value for seed, expected positive number");
            }
        } else if (opt == "arity") {
            if (!utils::parse_int(arity, val) || arity < 1) {// || arity % 2 != 0) {
                throw std::runtime_error("Invalid value for arity, expected positive number > 0");
            }
        } else if (opt == "mutate") {
            if (!utils::parse_int(mutation, val) || mutation < 1) {
                throw std::runtime_error("Invalid value for mutatation, expected number >= 1");
            }
        } else if (opt == "terms") {
            utils::parse_int(term_count, val);
        // } else if (opt == "crossover") {
        //     if (val != "true" && val != "false") {
        //         throw std::runtime_error("Invalid value for crossover-random, expected true or false");
        //     }
        //     crossover = (val == "true");
        } else if (opt == "print-used-gates") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for print-used-gates, expected true or false");
            }
            print_used_gates = (val == "true");
        } else if (opt == "print-used-area") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for print-used-area, expected true or false");
            }
            print_used_area = (val == "true");
        } else if (opt == "print-ascii") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for print-ascii, expected true or false");
            }
            print_ascii = (val == "true");
        } else {
            throw std::runtime_error("Invalid switch `" + opt + "`");
        }
    }
}

void Parameters::print_help() {
    std::cout << "anf help" << std::endl; // TODO
};

void Parameters::is_valid(const ReferenceBits &reference_bits) {
    int input_size = reference_bits.input.size();
    if (term_count == 0) {
        term_count = input_size;
    }
    // if (term_count < input_size) {
    //     throw std::runtime_error("Invalid value for terms, expected number >= " + std::to_string(input_size));
    // }
}