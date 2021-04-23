/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Parsing program's parameters. Futher info "how to use" in README.md can be found (boundaries)
 * 
 */

#include <string>
#include <iostream>
#include <ctime>
#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <string>
#include <exception>
#include <stdexcept>
#include "parameters.hpp"
#include "../utils.hpp"

Parameters::Parameters() {};

Parameters::Parameters(const int argc, const char * const argv[]) {
    if (argc % 2 != 1) {
        throw std::runtime_error("Invalid parameter count");
    }

    bool lback_all = false;
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
        } else if (opt == "row") {
            if (!utils::parse_int(row, val) || row < 1) {
                throw std::runtime_error("Invalid value for row, expected number >= 1");
            }
        } else if (opt == "column") {
            if (!utils::parse_int(column, val) || column < 1) {
                throw std::runtime_error("Invalid value for column, expected number >= 1");
            }
        } else if (opt == "mutate") {
            if (!utils::parse_int(mutation_rate, val) || mutation_rate < 0) {
                throw std::runtime_error("Invalid value for mutate rate, expected number >= 0");
            }
        } else if (opt == "print-count") {
            if (!utils::parse_int(print_count, val) || print_count < 1) {
                throw std::runtime_error("Invalid value for print-count, expected number >= 1");
            }
        } else if (opt == "lback") {
            if (val == "all") {
                lback_all = true;
            } else if (!utils::parse_int(level_back, val) || level_back < 1) {
                throw std::runtime_error("Invalid value for level-back, expected number >= 1");
            }
        } else if (opt == "seed") {
            if (!utils::parse_unsigned_int(seed, val)) {
                throw std::runtime_error("Invalid value for seed, expected positive number");
            }
        } else if (opt == "path") {
            path = val; // validity provides ReferenceBits object
        } else if (opt == "print-fitness") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for print-fitness, expected true or false");
            }
            print_fitness = (val == "true");
        } else if (opt == "second-criterion") {
            if (val != "true" && val != "false") {
                throw std::runtime_error("Invalid value for second-criterion, expected true or false");
            }
            second_criterion = (val == "true");
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
        } else if (opt == "functions") {
            parse_function_list(val);
        } else {
            throw std::runtime_error("Invalid switch option `" + opt + " " + val + "`");
        }
    }

    if (level_back > column) {
        throw std::runtime_error("Invalid level-back value, value has to be <= " + std::to_string(column));
    } else if (lback_all) {
        level_back = column;
    } else if (level_back == 0) {
        level_back = column;
    }
    /*     input1, input2, function => 3                       */
    mutation_rate = (((column * row) * 3) / 100) * mutation_rate;
}

void Parameters::function_append(const std::string &fun) {
    try {
        allowed_functions.push_back(function_list.at(fun));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Invalid function `" + fun + "`, allowed: in, not, and, or, xor, nand, nor, xnor");
    }
}

void Parameters::parse_function_list(const std::string &list) {
    allowed_functions.clear();
    auto fun_list = utils::split(list);

    for (auto &fun : fun_list) {
        boost::algorithm::to_lower(fun);
        function_append(fun);
    }

    if (utils::check_duplicates(allowed_functions)) {
        throw std::runtime_error("Function in list is duplicated");
    }

    if (allowed_functions.size() <= 1) {
        throw std::runtime_error("There is too little functions to work with");
    }
}

void Parameters::print_help() {
    std::cout << "Usage of CGP" << std::endl;
    std::cout << std::endl;
    std::cout << "Doesn't matter order" << std::endl;
    std::cout << "If there is any switch missing will be used default value" << std::endl;
    std::cout << "switch value => invalid value will be announced within each option" << std::endl;
    std::cout << std::endl;
    std::cout << "lambda 4" << std::endl;
    std::cout << "generations 100000" << std::endl;
    std::cout << "row 10" << std::endl;
    std::cout << "column 5" << std::endl;
    std::cout << "mutate 5" << std::endl;
    std::cout << "printcount 10000 (for effect need to be combined with switch below" << std::endl;
    std::cout << "printfitness true" << std::endl;
    std::cout << "lback all" << std::endl;
    std::cout << "seed 178846" << std::endl;
    std::cout << "path ./file.txt" << std::endl;
    std::cout << "functions or,and,not" << std::endl;
}