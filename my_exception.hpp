#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H
#include <string>
#include <exception>
#include <stdexcept>


class MyException : public std::runtime_error {
public:
    MyException(const std::string &msg = "") : std::runtime_error(msg) {}
};

#endif /* MY_EXCEPTION_H */