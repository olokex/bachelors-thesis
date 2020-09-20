#include <iostream>
#include <vector>
#include "parameters.hpp"
#include "cell.hpp"
#include "state.hpp"
#include "term.hpp"
#include "circuit.hpp"
#include "../reference_bits.hpp"
#include "../utils.hpp"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    //srand(15);
    Parameters p;
    //std::string path("/home/olok/bachelors-thesis/data/parity5.txt");
    //std::string path("/home/olok/bachelors-thesis/data/parity9.txt");
    //std::string path("/home/olok/bachelors-thesis/data/adder3_3.txt");
    std::string path("/home/olok/bachelors-thesis/data/median5.txt");
    ReferenceBits ref(path);
    std::vector<Circuit> population;

    for (int i = 0; i < p.lambda + 1; i++) {
        Circuit c(p.literal_count, ref);
        population.push_back(c);
    }
    Circuit fittest = utils::get_fittest_invidiual(population);
    population.pop_back();

    for (int i = 0; i < p.generations; i++) {
        for (int y = 0; y < p.lambda; y++) {
            population[y] = fittest;
            population[y].mutate(p.mutation, ref);
            population[y].calculate_fitness(p, ref);
            if (population[y].fitness == 0) {
                std::cout << "generation: " << i << std::endl;
                population[y].print_circuit(ref.input.size());
                return 0;
            }
        }
        Circuit &new_fittest = utils::get_fittest_invidiual(population);
        if (new_fittest.fitness <= fittest.fitness) {
            fittest = new_fittest;
        }
        if (i % 10000 == 0) {
            std::cout << "iter: " << i << " fitness: " << fittest.fitness << std::endl;
        }
    }
    std::cout << "NOT FOUND" << std::endl;
}