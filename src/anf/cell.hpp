#ifndef CELL_H
#define CELL_H
#include "../reference_bits.hpp"
#include "state.hpp"

class Cell {
public:
    Bitset value;
    State state;
};

#endif /* CELL_H */