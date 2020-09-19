#ifndef FUNCTION_H
#define FUNCTION_H
#include <string>

enum class Function {
    In,
    Not,
    And,
    Or,
    Xor,
    Nand,
    Nor,
    Xnor,
};

static inline std::string function_name(const Function &f) {
    switch (f) {
        case Function::In: return "in";
        case Function::Not: return "not";
        case Function::And: return "and";
        case Function::Or: return "or";
        case Function::Xor: return "xor";
        case Function::Nand: return "nand";
        case Function::Nor: return "nor";
        case Function::Xnor: return "xnor";
        default:
            return "unknown gate name - shouldn't appear in any way";
    }
}

#endif /* FUNCTION_H */