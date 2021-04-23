/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Constants for the size of a gate smallest possible element.
 *      Here is used 45 nm process.
 * 
 */

#ifndef PROCESS_SIZE_H
#define PROCESS_SIZE_H

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