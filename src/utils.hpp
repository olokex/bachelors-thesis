/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Header's file for utitility namespace.
 *      Some useful functions as check for duplicity, if vector contains an element, 
 *      function providing parent within better fitness/chip's area.
 * 
 */

#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <algorithm>

namespace utils {
    bool parse_unsigned_int(unsigned int &value, const std::string &str);
    bool parse_int(int &value, const std::string &str);
    std::vector<std::string> split(const std::string &functions, const char delimiter=',');
    int randint(const int bottom, const int top);

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

    template<class T>
    inline T &get_fittest_invidiual(std::vector<T> &population) {
        T *best = &population.at(0);
        for (size_t i = 1; i < population.size(); i++) {
            if (population[i].fitness <= best->fitness) {
                best = &population[i];
            }
        }
        return *best;
    }

    template<class T>
    inline T &get_fittest_invidiual_area(std::vector<T> &population) {
        T *best = &population.at(0);
        for (size_t i = 1; i < population.size(); i++) {
            if (population[i].fitness <= best->fitness && population[i].area <= best->area) {
                best = &population[i];
            }
        }
        return *best;
    }
}

#endif /* UTILS_H */