/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Class for parametrs, which store default parameters for evo run.
 *      All parameters can be modified within program parameters.
 * 
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <vector>
#include <string>
#include <map>
#include "function.hpp"
#include "../utils.hpp"

class Parameters {
public:
    std::vector<Function> allowed_functions = {
        Function::In,
        Function::Not,
        Function::And,
        Function::Or,
        Function::Xor,
        Function::Nand,
        Function::Nor,
        Function::Xnor
    };
    int lambda = 4;
    int row = 1;
    int column = 80;
    int mutation_rate = 5;
    int level_back = 0;
    unsigned int generations = 5'000'000;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;
    bool print_used_gates = false;
    bool print_used_area = false;
    bool second_criterion = false;
    int print_count = 100'000;

    Parameters();
    Parameters(const int argc, const char * const argv[]);
    static void print_help();

private:
    const std::map<std::string, Function> function_list = {
        {"in", Function::In},
        {"not", Function::Not},
        {"and", Function::And},
        {"or", Function::Or},
        {"xor", Function::Xor},
        {"nand", Function::Nand},
        {"nor", Function::Nor},
        {"xnor", Function::Xnor}
    };

    void function_append(const std::string &fun);
    void parse_function_list(const std::string &list);
};

#endif /* PARAMETERS_H */