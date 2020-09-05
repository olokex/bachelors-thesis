#ifndef REFERENCE_BITS_H
#define REFERENCE_BITS_H
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>

class ReferenceBits {
public:
    std::vector<boost::dynamic_bitset<>> input;
    std::vector<boost::dynamic_bitset<>> output;
    ReferenceBits(const std::string &path);

private:
    std::vector<std::string> remove_unnecessary(std::ifstream &fp);
    void input_append(const std::string bits);
    void output_append(const std::string bits);
};

#endif /* REFERENCE_BITS_H */