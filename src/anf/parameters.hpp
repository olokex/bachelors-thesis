#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>

class Parameters {
public:
    int lambda = 4;
    int literal_count = 10;
    int mutation = 2;
    int uniform_mutation = 1;
    unsigned int generations = 5'000'000;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;
    bool print_used_gates = false;
    bool print_ascii = false;
    int print_count = 100'000;

    Parameters();
    Parameters(const int argc, const char * const argv[]);
    static void print_help();
};

#endif /* PARAMETERS_H */