/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Header's file for bitsets.
 * 
 */

#ifndef REFERENCE_BITS_H
#define REFERENCE_BITS_H
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>

using Bitset = boost::dynamic_bitset<>;

class ReferenceBits {
public:
    std::vector<Bitset> input;
    std::vector<Bitset> output;
    ReferenceBits(const std::string &path);

private:
    std::vector<std::string> remove_unnecessary(std::ifstream &fp);
    void input_append(const std::string &bits);
    void output_append(const std::string &bits);
};

#endif /* REFERENCE_BITS_H */