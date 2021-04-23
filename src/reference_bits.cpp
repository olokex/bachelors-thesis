/**
 * Subject: Bachelor's thesis
 * Author: Adam Sedlacek | xsedla1e@vutbr.cz
 * Year: 2021
 * Description:
 *      Parsing inputs truth table to obtain reference bitset.
 * 
 */

#include "reference_bits.hpp"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>


/**
 * Parses input file to obtain bitset vector. Supported format is PLA standard.
 * .header
 * Xn inputs space Yn outputs
 * 
 * 1 logical true
 * 0 logical false
 * - input's don't care
 * ~ output's don't care
 */
ReferenceBits::ReferenceBits(const std::string &path) {
    std::ifstream fp;
    std::string line;
    fp.open(path);

    if (!fp.is_open()) {
        throw std::runtime_error("opening file `" + path + "` failed");
    }

    std::vector<std::string> file_clear = remove_unnecessary(fp);

    line = file_clear.front();
    size_t delimiter = line.find(' ');

    for (size_t col = 0; col < line.size(); col++) {
        if (col == delimiter) continue;
        std::string bits;
        for (size_t row = 0; row < file_clear.size(); row++) {
            switch (file_clear[row][col]) {
                case '1':
                    bits += '1';
                    break;
                case '0':
                    bits += '0';
                    break;
                case '-':
                    bits += '0';
                    break;
                case '~':
                    bits += '0';
                    break;
                default:
                    throw std::runtime_error("unknown character `" + std::string(1, file_clear[row][col]) + " supported format is PLA");
            }
        }
        if (col < delimiter) {
            input_append(bits);
        } else {
            output_append(bits);
        }
    }
}

/**
 * Method removes needless inputs as a header of PLA.
 */
std::vector<std::string> ReferenceBits::remove_unnecessary(std::ifstream &fp) {
    std::string line;
    std::vector<std::string> clean_file;

    while (std::getline(fp, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        size_t pos = line.find('.');
        if (pos != std::string::npos) {
            line = line.substr(0, pos);
        }
        if (line.size() == 0) continue;
        clean_file.push_back(line);
    }
    return clean_file;
}

void ReferenceBits::input_append(const std::string &bits) {
    input.push_back(Bitset(bits));
}

void  ReferenceBits::output_append(const std::string &bits) {
    output.push_back(Bitset(bits));
}