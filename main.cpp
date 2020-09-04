#include <iostream>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>

class MyException : public std::runtime_error {
public:
    MyException(const std::string &msg = "") : std::runtime_error(msg) {}
};

class ReferenceBits {
public:
    std::vector<boost::dynamic_bitset<>> input;
    std::vector<boost::dynamic_bitset<>> output;

    ReferenceBits(const std::string &path) {
        std::ifstream fp;
        std::string line;
        fp.open(path);

        if (!fp.is_open()) {
            throw MyException("opening file `" + path + "` failed");
        }

        std::vector<std::string> file_clear = remove_unnecessary(fp);

        line = file_clear.front();
        size_t delimiter = line.find(':');

        for (int col = 0; col < line.size(); col++) {
            if (col == delimiter) continue;
            std::string bits;
            for (int row = 0; row < file_clear.size(); row++) {
                bits += file_clear[row][col];
            }
            if (col < delimiter) {
                input_append(bits);
            } else {
                output_append(bits);
            }
        }
    }

private:
    std::vector<std::string> remove_unnecessary(std::ifstream &fp) {
        std::string line;
        std::vector<std::string> clean_file;

        while (std::getline(fp, line)) {
            size_t pos = line.find('#');
            if (pos != std::string::npos) {
                line = line.substr(0, pos);
            }
            line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
            if (line.size() == 0) continue;
            clean_file.push_back(line);
        }
        return clean_file;
    }

    void input_append(const std::string bits) {
        input.push_back(boost::dynamic_bitset<>(bits));
    }

    void output_append(const std::string bits) {
        output.push_back(boost::dynamic_bitset<>(bits));
    }
};

enum class Function {
    In,
    Not,
    And,
    Or,
    Xor,
    Nand,
    Nor,
    Xnor,
    NumberOfFunctions
};

class Cell {
public:
    unsigned short int input1;
    unsigned short int input2;
    Function function;
    boost::dynamic_bitset<> output;
};

class Parameters {
public:
    std::vector<Function> allowed_functions = {
        Function::In,
        Function::Not,
        Function::And,
        Function::Or,
        Function::Xor,
        Function::Nand,
        Function::Nor,
        Function::Xnor
    };
    int lambda = 5;
    int generations = 5'000'000;
    int row = 1;
    int column = 80;
    int mutate_components = 5;
    int seed = time(nullptr);

    Parameters() {};
    Parameters(int argc, char *argv[]) {

        // TODO HELP?
        if (argc % 2 != 1) {
            throw MyException("invalid switch option");
        }
        
        for (int i = 1; i < argc; i += 2) {
            std::string opt(argv[i]);
            std::string val(argv[i + 1]);
            if (opt == "lambda") lambda = parse_number(val);
            else if (opt == "generations") generations = parse_number(val);
            else if (opt == "row") row = parse_number(val);
            else if (opt == "column") column = parse_number(val);
            else if (opt == "mutate") mutate_components = parse_number(val, 0);
            else if (opt == "seed") seed = parse_number(val, 0);
            else if (opt == "function") parse_function_list(val);
            else { throw MyException("wrong switch option"); }
        }
    }

private:
    const std::map<std::string, Function> function_list = {
        {"in", Function::In},
        {"not", Function::Not},
        {"and", Function::And},
        {"or", Function::Or},
        {"xor", Function::Xor},
        {"nand", Function::Nand},
        {"nor", Function::Nor},
        {"xnor", Function::Xnor}
    };

    void function_append(const std::string &fun) {
        try {
            allowed_functions.push_back(function_list.at(fun));
        } catch (std::out_of_range) {
            throw MyException("invalid function " + fun + ", allowed: in, not, and, or, xor, nand, nor, xnor");
        }
    }

    void check_duplicate() {
        for (auto &fun : allowed_functions) {
            int occur = std::count(allowed_functions.begin(), allowed_functions.end(), fun);
            if (occur > 1) {
                throw MyException("function in list is duplicated");
            }
        }
    }

    std::vector<std::string> split(const std::string &functions, const char delimiter=',') {
        std::stringstream ss(functions);
        std::string token;
        std::vector<std::string> list;
        while(std::getline(ss, token, delimiter)) {
            list.push_back(token);
        }
        return list;
    }

    void parse_function_list(const std::string &list) {
        allowed_functions.clear();
        auto fun_list = split(list);

        for (auto &fun : fun_list) {
            boost::algorithm::to_lower(fun);
            function_append(fun);
        }

        check_duplicate();
        if (allowed_functions.size() <= 1) {
            throw MyException("there is too little functions to work with");
        }
    }

    int parse_number(const std::string &value, const int lowest = 1) {
        try {
            int num = std::stoi(value);
            if (num < lowest) throw std::range_error("");
            return num;
        } catch (std::invalid_argument) {
            throw MyException("not a number, bad switch value " + value);
        } catch (std::range_error) {
            throw MyException("number is in wrong range " + value);
        }
    }
};


int main(int argc, char *argv[]) {
    try {
        //std::string path("/home/olok/cgp_64bit/data/multiplier2x2.txt");
        std::string path("/home/olok/cgp_64bit/data/parity5.txt");
        //ReferenceBits reference_bits = parse_file(path);
        ReferenceBits reference_bits(path);
        Parameters params(argc, argv);
    } catch (const MyException &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }

}
