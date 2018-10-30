#include "operators.h"

#include <unordered_map>

#include "lexer/token.h"

static const std::unordered_map<Token, int> BINOP_PRECEDENCE {
    {tok_equal, 2},
    {tok_less, 10},
    {tok_greater, 10},
    {tok_greatequal, 10},
    {tok_lessequal, 10},
    {tok_add, 20},
    {tok_sub, 20},
    {tok_mul, 40},
    {tok_div, 40},
};

int getTokenPrecedence(Token t) {
    auto precedence = BINOP_PRECEDENCE.find(t);
    if (precedence == BINOP_PRECEDENCE.end()) {
        return -1;
    }
    return precedence->second;
}
