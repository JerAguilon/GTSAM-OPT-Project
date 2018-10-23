#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <vector>

#include "lexer/token.h"

namespace lexer {
    Token& make_token(
        std::vector<Token>& tokens, const char *filename, TokenType tok_type, int line, int col
    );

    int tokenize_stream(std::istream& file, const char*fname, std::vector<Token>& tokens);
}

extern int CurTok;
extern std::string IdentifierStr;
extern double NumVal;

int gettok();
int getNextToken();


#endif
