/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Literal class hold bitset according index of input, and its state (active, inactive, negated).
 * 
 */
#ifndef LITERAL_H
#define LITERAL_H
#include "../reference_bits.hpp"
#include "state.hpp"

class Literal {
public:
    Bitset value;
    State state;
};

#endif /* LITERAL_H */