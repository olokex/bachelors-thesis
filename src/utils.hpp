#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

namespace utils {
    void print_help();
    bool parse_unsigned_int(unsigned int &value, const std::string &str);
    bool parse_int(int &value, const std::string &str);
    std::vector<std::string> split(const std::string &functions, const char delimiter=',');
}

#endif /* UTILS_H */