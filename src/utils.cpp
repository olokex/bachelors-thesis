#include "utils.hpp"
#include "my_exception.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

bool utils::parse_int(int &value, const std::string &str) {
    try {
        value = boost::lexical_cast<int>(str);
        return true;
    } catch (boost::bad_lexical_cast) {
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
        } catch (boost::bad_lexical_cast) {
            return false;
        }
    }

void utils::print_help() {
    std::cout << "help" << std::endl;
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