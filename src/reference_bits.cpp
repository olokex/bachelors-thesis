#include "reference_bits.hpp"
#include "my_exception.hpp"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

ReferenceBits::ReferenceBits(const std::string &path) {
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

std::vector<std::string> ReferenceBits::remove_unnecessary(std::ifstream &fp) {
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

void ReferenceBits::input_append(const std::string bits) {
    input.push_back(boost::dynamic_bitset<>(bits));
}

void  ReferenceBits::output_append(const std::string bits) {
    output.push_back(boost::dynamic_bitset<>(bits));
}