#ifndef __TOKEN_H__
#define __TOKEN_H__

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

    // TODO: custom control flows <= etc.
    tok_lessequal = -9,
    tok_greatequal = -10,

    // loop controls
    tok_for = -11,
    tok_in = -12,
};

#endif
