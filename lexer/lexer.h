#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>

extern int CurTok;
extern std::string IdentifierStr;
extern double NumVal;

int gettok();
int getNextToken();


#endif
