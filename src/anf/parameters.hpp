#ifndef PARAMETERS_H
#define PARAMETERS_H
#include "../reference_bits.hpp"
#include <string>

class Parameters {
public:
    int arity = 2;
    int lambda = 4;
    int term_count = 0;
    int mutation = 1;
    int mutation_row = 3;
    int uniform_mutation = 1;
    bool crossover = false;
    unsigned int generations = 5'000'000;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;
    bool print_used_gates = false;
    bool print_used_area = false;
    bool print_ascii = false;
    int print_count = 100'000;

    Parameters();
    Parameters(const int argc, const char * const argv[]);
    void is_valid(const ReferenceBits &reference_bits);
    static void print_help();
};

#endif /* PARAMETERS_H */