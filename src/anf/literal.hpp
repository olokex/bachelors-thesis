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