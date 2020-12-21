#ifndef PROCESS_SIZE_H
#define PROCESS_SIZE_H

/**
 * Describes the size of a gate smallest possible element.
 * Here is used 45 nm process.
 */
namespace gate_size {
    constexpr double Not  = 1.40;
    constexpr double And  = 2.34;
    constexpr double Or   = 2.34;
    constexpr double Xor  = 4.69;
    constexpr double Nand = 1.87;
    constexpr double Nor  = 2.34;
    constexpr double Xnor = 4.69;
}

#endif /* PROCESS_SIZE_H */