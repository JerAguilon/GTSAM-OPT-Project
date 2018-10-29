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

    tok_lessequal = -9,
    tok_greatequal = -10,

    // loop controls
    tok_for = -11,
    tok_in = -12,
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
