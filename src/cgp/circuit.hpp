#ifndef CIRCUIT_H
#define CIRCUIT_H

class Circuit {
public:
    unsigned int fitness = UINT_MAX;

    Circuit(const Parameters &param, const ReferenceBits &reference_bits);
    void print_circuit_cgpviewer(const Parameters &param, const ReferenceBits &reference_bits);
    void evaluate(const int &input_size);
    void calculate_fitness(const ReferenceBits &reference_bits);
    void mutate(const int &mutation_rate, const std::vector<Function> &allowed_functions, const int &inputs_count);
    void print_bits(const ReferenceBits &reference_bits);
    void print_used_gates(const int &inputs_count, const std::vector<Function> &allowed);

private:
    std::vector<Cell> cells;
    std::vector<int> output_indices;
    std::vector<int> used_gates_indices;
    int inputs_count;
    int delta = 0;

    void push_inputs(const ReferenceBits &reference_bits);
    int count_gates_within_function(const Function &fun);
    bool redundant_gate(const int &idx);
    void add_pass_gate(const int &idx);
    void find_used_gates();
    int depth(const int &idx);
    void find_max_delay();
};

#endif /* CIRCUIT_H */