#include "parameters.hpp"
#include <string>
#include <ctime>
#include <map>
#include "my_exception.hpp"
#include "utils.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>

Parameters::Parameters() {};

Parameters::Parameters(const int argc, const char * const argv[]) {
    if (argc % 2 != 1) {
        throw CGPException("Invalid parameter count");
    }

    bool lback_all = false;
    for (int i = 1; i < argc; i += 2) {
        std::string opt(argv[i]);
        std::string val(argv[i + 1]);
        if (opt == "lambda") {
            if (!utils::parse_unsigned_int(lambda, val) || lambda < 1) {
                throw CGPException("Invalid value for lambda, expected number >= 1");
            }
        } else if (opt == "generations") {
            if (!utils::parse_unsigned_int(generations, val) || generations < 1) {
                throw CGPException("Invalid value for generations, expected number >= 1");
            }
        } else if (opt == "row") {
            if (!utils::parse_unsigned_int(row, val) || row < 1) {
                throw CGPException("Invalid value for row, expected number >= 1");
            }
        } else if (opt == "column") {
            if (!utils::parse_unsigned_int(column, val) || column < 1) {
                throw CGPException("Invalid value for column, expected number >= 1");
            }
        } else if (opt == "mutate") {
            if (!utils::parse_unsigned_int(mutation_rate, val) || mutation_rate < 0) {
                throw CGPException("Invalid value for mutate rate, expected number >= 0");
            }
        } else if (opt == "print-count") {
            if (!utils::parse_unsigned_int(print_count, val) || print_count < 1) {
                throw CGPException("Invalid value for print-count, expected number >= 1");
            }
        } else if (opt == "lback") {
            if (val == "all") {
                lback_all = true;
            } else if (!utils::parse_unsigned_int(level_back, val) || level_back < 1) {
                throw CGPException("Invalid value for level-back, expected number >= 1");
            }
        } else if (opt == "seed") {
            if (!utils::parse_unsigned_int(seed, val)) {
                throw CGPException("Invalid value for seed, expected positive number");
            }
        } else if (opt == "path") {
            path = val; // validity provides ReferenceBits object
        } else if (opt == "print-fitness") {
            if (val != "true" && val != "false") {
                throw CGPException("Invalid value for print-fitness, expected true or false");
            }
            print_fitness = (val == "true");
        } else if (opt == "print-used-gates") {
            if (val != "true" && val != "false") {
                throw CGPException("Invalid value for print-used-gates, expected true or false");
            }
            print_used_gates = (val == "true");
        } else if (opt == "functions") {
            parse_function_list(val);
        } else {
            throw CGPException("Invalid switch option `" + opt + " " + val + "`");
        }
    }

    if (level_back > column) {
        throw CGPException("Invalid level-back value, value has to be <= " + std::to_string(column));
    } else if (lback_all) {
        level_back = column;
    } else if (level_back == 0) {
        level_back = column;
    }
}

void Parameters::function_append(const std::string &fun) {
    try {
        allowed_functions.push_back(function_list.at(fun));
    } catch (std::out_of_range) {
        throw CGPException("Invalid function `" + fun + "`, allowed: in, not, and, or, xor, nand, nor, xnor");
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
        throw CGPException("Function in list is duplicated");
    }

    if (allowed_functions.size() <= 1) {
        throw CGPException("There is too little functions to work with");
    }
}