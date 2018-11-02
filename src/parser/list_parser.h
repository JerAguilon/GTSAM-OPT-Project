#ifndef LIST_PARSER_H
#define LIST_PARSER_H

#include <map>
#include <vector>

#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/ForExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/IfExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"
#include "lexer/lexer.h"
#include "lexer/token.h"

class ListParser {
    std::vector<TokenWrapper> tokens;
    int curr_token;

public:
    ListParser(std::vector<TokenWrapper>& tokens) :
        tokens(tokens), curr_token(0)
    {}
    
    std::unique_ptr<ExprAST> parse();
private:
    std::unique_ptr<ExprAST> parseNumberExpr();
    std::unique_ptr<ExprAST> parseParenExpr();
    std::unique_ptr<ExprAST> parseIdentifierExpr();
    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<PrototypeAST> parsePrototype();
    std::unique_ptr<FunctionAST> parseDefinition();
    std::unique_ptr<FunctionAST> parseTopLevelExpr();
    std::unique_ptr<PrototypeAST> parseExtern();
    std::unique_ptr<ExprAST> parseIfExpr();
    std::unique_ptr<ExprAST> parseForExpr();
};


#endif
