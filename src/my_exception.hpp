#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H
#include <string>
#include <exception>
#include <stdexcept>


class CGPException : public std::runtime_error {
public:
    CGPException(const std::string &msg = "") : std::runtime_error(msg) {}
};

#endif /* MY_EXCEPTION_H */