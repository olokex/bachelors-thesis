/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Useful functions can be found in utitility namespace, such as cast u/int.
 *      Split function (same as in python).
 *      Randint function (same as in python with excluded upper limit).
 * 
 */

#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "utils.hpp"

bool utils::parse_int(int &value, const std::string &str) {
    try {
        value = boost::lexical_cast<int>(str);
        return true;
    } catch (const boost::bad_lexical_cast&) {
        return false;
    }
}

bool utils::parse_unsigned_int(unsigned int &value, const std::string &str) {
        try {
            value = boost::lexical_cast<unsigned int>(str);
            if (str.find('-') != std::string::npos) {
                throw boost::bad_lexical_cast();
            }
            return true;
        } catch (const boost::bad_lexical_cast&) {
            return false;
        }
    }

std::vector<std::string> utils::split(const std::string &functions, const char delimiter) {
    std::stringstream ss(functions);
    std::string token;
    std::vector<std::string> list;
    while(std::getline(ss, token, delimiter)) {
        list.push_back(token);
    }
    return list;
}

int utils::randint(const int bottom, const int top) {
    if (bottom >= top) throw std::invalid_argument("Bad range for randint A < B");
    //return rand() % (top - (bottom - 1)) + bottom; // includes top and bot
    return rand() % (top - bottom) + bottom; // excludes top
}