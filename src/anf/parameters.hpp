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
#include "../reference_bits.hpp"
#include <string>

class Parameters {
public:
    unsigned int arity = 2;
    int lambda = 4;
    int term_count = 0;
    int mutation = 2;
    unsigned int generations = 5'000'000;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;
    bool print_used_gates = false;
    bool print_used_area = false;
    bool print_ascii = false;
    bool second_criterion = false;
    int print_count = 100'000;

    Parameters();
    Parameters(const int argc, const char * const argv[]);
    void is_valid(const ReferenceBits &reference_bits);
    static void print_help();
};

#endif /* PARAMETERS_H */