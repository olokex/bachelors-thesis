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
    int mutate = 5;
    int level_back = 0;
    unsigned int seed = time(nullptr);
    std::string path;
    bool print_fitness = false;

    Parameters() {};
    Parameters(const int argc, const char * const argv[]) {
        if (argc % 2 != 1) {
            throw MyException("invalid parameters count");
        }
        bool lback_all = false;
        for (int i = 1; i < argc; i += 2) {
            std::string opt(argv[i]);
            std::string val(argv[i + 1]);
            if (opt == "lambda") {
                if (!parse_int(lambda, val) || lambda < 1) {
                    throw MyException("Invalid value for lambda, expected number >= 1");
                }
            } else if (opt == "generations") {
                if (!parse_int(generations, val) || generations < 1) {
                    throw MyException("Invalid value for generations, expected number >= 1");
                }
            } else if (opt == "row") {
                if (!parse_int(row, val) || row < 1) {
                    throw MyException("Invalid value for row, expected number >= 1");
                }
            } else if (opt == "column") {
                if (!parse_int(column, val) || column < 1) {
                    throw MyException("Invalid value for column, expected number >= 1");
                }
            } else if (opt == "mutate") {
                if (!parse_int(mutate, val) || mutate < 0) {
                    throw MyException("Invalid value for mutate, expected number >= 0");
                }
            } else if (opt == "lback") {
                if (val == "all") {
                    lback_all = true;
                } else if (!parse_int(level_back, val) || level_back < 1) {
                    throw MyException("Invalid value for level-back, expected number >= 1");
                }
            } else if (opt == "seed") {
                if (!parse_unsigned_int(seed, val)) {
                    throw MyException("Invalid value for seed, expected positive number");
                }
            } else if (opt == "path") {
                path = val; // validity provides reference bits object
            } else if (opt == "printfitness") {
                if (val != "true" && val != "false") {
                    throw MyException("Invalid value for printfitness, expected true or false");
                }
                print_fitness = (val == "true");
            } else if (opt == "functions") {
                parse_function_list(val);
            } else {
                throw MyException("Invalid switch option");
            }
        }

        if (lback_all) {
            level_back = column;
        } else if (!(level_back <= column)) {
            throw MyException("Invalid level-back value, value has to be <= " + std::to_string(column));
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

    bool parse_int(int &value, const std::string &str) {
        try {
            value = boost::lexical_cast<int>(str);
            return true;
        } catch (boost::bad_lexical_cast) {
            return false;
        }
    }

    bool parse_unsigned_int(unsigned int &value, const std::string &str) {
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
};

void print_help() {
    // TODO
    std::cout << "help me" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        //std::string path("/home/olok/cgp_64bit/data/multiplier2x2.txt");
        std::string path("/home/olok/cgp_64bit/data/parity5.txt");
        //ReferenceBits reference_bits = parse_file(path);
        if (argc == 2 && std::string(argv[1]) == "-h") {
            print_help();
            return 0;
        }
        //ReferenceBits reference_bits(path);
        Parameters params(argc, argv);
        std::cout << params.column << std::endl;
        std::cout << params.print_fitness << std::endl;
    } catch (const MyException &err) {
        std::cerr << "ERROR: " << err.what() << std::endl;
        return 1;
    }

}
