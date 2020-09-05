#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <vector>
#include <string>
#include <map>
#include "function.hpp"

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
    int lambda = 5;
    int generations = 5'000'000;
    int row = 1;
    int column = 80;
    int mutate = 5;
    int level_back = 0;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;

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

    void check_duplicate();
    void function_append(const std::string &fun);
    void parse_function_list(const std::string &list);
};

#endif /* PARAMETERS_H */