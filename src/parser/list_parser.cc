#include "logger/logger.h"
#include "parser/list_parser.h"
#include "utils/operators.h"

#include <boost/format.hpp>


std::unique_ptr<ExprAST> ListParser::parse() {

}

std::unique_ptr<ExprAST> ListParser::parseNumberExpr() {
    auto t_wrapper = tokens[curr_token++];
    auto result = llvm::make_unique<NumberExprAST>(t_wrapper.value);
    return std::move(result);
}

std::unique_ptr<ExprAST> ListParser::parseParenExpr() {
    curr_token++; // eat the '('

    auto V = parseExpression();
    if (!V) return nullptr;

    if (tokens[curr_token].type != tok_rparen) return LogError("Expected ')'");

    curr_token++; // eat the ')'
    return V;
}

std::unique_ptr<ExprAST> ListParser::parseIdentifierExpr() {
    std::string id = tokens[curr_token].str_content;
    curr_token++;

    // Case 1: It's a a variable expression, not a method call
    if (tokens[curr_token].type != tok_lparen) {
        return llvm::make_unique<VariableExprAST>(id);
    }

    curr_token++; // eat the '('

    std::vector<std::unique_ptr<ExprAST>> arguments;
    if (tokens[curr_token].type != tok_rparen) {
        while (true) {
            if (auto argument = parseExpression()) {
                arguments.push_back(std::move(argument));
            } else {
                return nullptr;
            }

            if (tokens[curr_token].type == tok_rparen) {
                break;
            } else if (tokens[curr_token].type == tok_comma) {
                curr_token++; // eat the comma
            } else {
                return LogError("Expected ')' or ',' in the argument list");
            }

        }
    }

    curr_token++; // eat the ')'
    return llvm::make_unique<CallExprAST>(id, std::move(arguments));
}

std::unique_ptr<ExprAST> ListParser::parsePrimary() {
    switch (tokens[curr_token].type) {
        default: return LogError(
            (boost::format("Unknown token when expecting an expression: %1%") % tokens[curr_token].type).str().c_str()
        );
        case tok_identifier: return parseIdentifierExpr();
        case tok_number: return parseNumberExpr();
        case tok_lparen: return parseParenExpr();
        case tok_if: return parseIfExpr();
        case tok_for: return parseForExpr();
    }
}

std::unique_ptr<ExprAST> ListParser::parseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {

}

std::unique_ptr<ExprAST> ListParser::parseExpression() {

}

std::unique_ptr<PrototypeAST> ListParser::parsePrototype() {

}

std::unique_ptr<FunctionAST> ListParser::parseDefinition() {

}

std::unique_ptr<FunctionAST> ListParser::parseTopLevelExpr() {

}

std::unique_ptr<PrototypeAST> ListParser::parseExtern() {

}

std::unique_ptr<ExprAST> ListParser::parseIfExpr() {

}

std::unique_ptr<ExprAST> ListParser::parseForExpr() {

}

