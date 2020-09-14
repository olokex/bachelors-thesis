#include "utils.hpp"
#include "my_exception.hpp"
#include "function.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

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
    std::cout << "Usage of CGP" << std::endl;
    std::cout << std::endl;
    std::cout << "Doesn't matter order" << std::endl;
    std::cout << "If there is any switch missing will be used default value" << std::endl;
    std::cout << "switch value => invalid value will be announced within each option" << std::endl;
    std::cout << std::endl;
    std::cout << "lambda 4" << std::endl;
    std::cout << "generations 100000" << std::endl;
    std::cout << "row 10" << std::endl;
    std::cout << "column 5" << std::endl;
    std::cout << "mutate 5" << std::endl;
    std::cout << "printcount 10000 (for effect need to be combined with switch below" << std::endl;
    std::cout << "printfitness true" << std::endl;
    std::cout << "lback all" << std::endl;
    std::cout << "seed 178846" << std::endl;
    std::cout << "path ./file.txt" << std::endl;
    std::cout << "functions or,and,not" << std::endl;
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

int utils::randint(const int &bottom, const int &top) {
    if (bottom >= top) throw std::invalid_argument("Bad range for randint A < B");
    //return rand() % (top - (bottom - 1)) + bottom; // includes top and bot
    return rand() % (top - bottom) + bottom; // excludes top
}

std::string utils::function_name(const Function &f) {
    if (f == Function::In) return "in";
    if (f == Function::Not) return "not";
    if (f == Function::And) return "and";
    if (f == Function::Or) return "or";
    if (f == Function::Xor) return "xor";
    if (f == Function::Nand) return "nand";
    if (f == Function::Nor) return "nor";
    if (f == Function::Xnor) return "xnor";
}