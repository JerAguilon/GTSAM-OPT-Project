#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,

    // control flows
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,

    // comparators
    tok_lessequal = -9,
    tok_greatequal = -10,
    tok_less = -11,
    tok_greater = -12,

    // operators
    tok_sub = -13,
    tok_add = -14,
    tok_div = -15,
    tok_mul = -16,

    // loop controls
    tok_for = -17,
    tok_in = -18,

    // assignment
    tok_equal = -19,

    tok_semicolon = -20,
};



struct TokenWrapper {
    int type; // TODO: replace this with type `Token`
    float value;
    std::string str_content;
    char *filename;
    int line_number;
    int column_number;
};

#endif
