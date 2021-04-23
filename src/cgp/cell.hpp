/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Cell coresponds to a gate in CGP graph/grid.
 * 
 */

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