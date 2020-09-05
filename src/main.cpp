#include <iostream>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include "my_exception.hpp"
#include "reference_bits.hpp"
#include "utils.hpp"
#include "function.hpp"
#include "parameters.hpp"

class Cell {
public:
    unsigned short int input1;
    unsigned short int input2;
    Function function;
    boost::dynamic_bitset<> output;
};

int main(int argc, char *argv[]) {
    try {
        //std::string path("/home/olok/cgp_64bit/data/multiplier2x2.txt");
        std::string path("/home/olok/cgp_64bit/data/parity5.txt");
        //ReferenceBits reference_bits = parse_file(path);
        if (argc == 2 && std::string(argv[1]) == "-h") {
            utils::print_help();
            return 0;
        }
        ReferenceBits reference_bits(path);
        Parameters params(argc, argv);
        std::cout << params.column << std::endl;
        std::cout << params.print_fitness << std::endl;
    } catch (const MyException &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }

}
