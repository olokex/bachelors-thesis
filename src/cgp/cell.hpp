#ifndef CELL_H
#define CELL_H
#include "../reference_bits.hpp"
#include "function.hpp"

class Cell {
public:
    int input1;
    int input2;
    Function function;
    Bitset output;
};

#endif /* CELL_H */