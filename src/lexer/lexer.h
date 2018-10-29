#ifndef __LEXER_H__
#define __LEXER_H__

#include "lexer/token.h"

#include <string>
#include <vector>

extern Token CurTok;
extern std::string IdentifierStr;
extern double NumVal;

Token gettok();
int getNextToken();

std::vector<TokenWrapper>& tokenizeStream(std::istream& infile, const char* fname, std::vector<TokenWrapper>& tokens);

#endif
