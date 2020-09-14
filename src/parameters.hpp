#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <vector>
#include <string>
#include <map>
#include "function.hpp"
#include "utils.hpp"

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
    unsigned int lambda = 4;
    unsigned int generations = 5'000'000;
    unsigned int row = 1;
    unsigned int column = 80;
    unsigned int mutation_rate = 5;
    unsigned int level_back = 0;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;
    bool print_used_gates = false;
    unsigned int print_count = 100'000;

    Parameters();
    Parameters(const int argc, const char * const argv[]);

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