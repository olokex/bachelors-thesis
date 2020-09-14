#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <algorithm>
#include "function.hpp"

namespace utils {
    void print_help();
    bool parse_unsigned_int(unsigned int &value, const std::string &str);
    bool parse_int(int &value, const std::string &str);
    std::vector<std::string> split(const std::string &functions, const char delimiter=',');
    int randint(const int &bottom, const int &top);
    std::string function_name(const Function &f);

    template<class T>
    bool check_duplicates(const std::vector<T> &vec) {
        for (auto &item : vec) {
            int occur = std::count(vec.begin(), vec.end(), item);
            if (occur > 1) return true;
        }
        return false;
    }

    template<class T>
    bool is_in_vector(const std::vector<T> &vec, const T &item) {
        int occur = std::count(vec.begin(), vec.end(), item);
        if (occur > 0) return true;
        return false;
    }
}

#endif /* UTILS_H */