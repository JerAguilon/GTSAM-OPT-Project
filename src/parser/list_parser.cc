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

std::unique_ptr<ExprAST> ListParser::parseBinOpRHS(int expression_precedence, std::unique_ptr<ExprAST> left_side) {
    while (true) {
        int precedence = getTokenPrecedence(tokens[curr_token].type);

        if (precedence < expression_precedence) {
            return left_side;
        }
        Token op = tokens[curr_token++].type;
        
        auto right_side = parsePrimary();
        if (!right_side) return nullptr;

        int next_precedence = getTokenPrecedence(tokens[curr_token].type);
        if (precedence < next_precedence) {
            right_side = parseBinOpRHS(precedence + 1, std::move(right_side));
            if (!right_side) return nullptr;
        }
        left_side = llvm::make_unique<BinaryExprAST>(op, std::move(left_side), std::move(right_side));
    }
}

std::unique_ptr<ExprAST> ListParser::parseExpression() {
    auto left_side = parsePrimary();
    if (!left_side) return nullptr;

    return parseBinOpRHS(0, std::move(left_side));
}

std::unique_ptr<PrototypeAST> ListParser::parsePrototype() {
    // Step 1: parse the function name
    auto token = tokens[curr_token];
    
    if (token.type != tok_identifier) {
        return LogErrorP("Expected function name in prototype");
    }

    std::string function_name = token.str_content;

    curr_token++;
    if (tokens[curr_token].type != tok_lparen) {
        return LogErrorP("Expected '(' in prototype");
    }
    curr_token++; // eat the '('

    std::vector<std::string> argument_names;
    auto argument = tokens[curr_token];
    while (argument.type == tok_identifier) {
        argument_names.push_back(argument.str_content);
        // TODO: should we comma delimit the arguments?
        curr_token++;
        argument = tokens[curr_token];
    }

    if (tokens[curr_token].type != tok_rparen) {
        return LogErrorP("Expected ')' in prototype");
    }

    curr_token++; // eat the ')'
    return llvm::make_unique<PrototypeAST>(function_name, std::move(argument_names));
}

std::unique_ptr<FunctionAST> ListParser::parseDefinition() {
    curr_token++; // eat the 'def'
    auto prototype = parsePrototype();    
    if (!prototype) return nullptr;

    if (auto expression = parseExpression()) {
        return llvm::make_unique<FunctionAST>(std::move(prototype), std::move(expression));
    }
}

std::unique_ptr<FunctionAST> ListParser::parseTopLevelExpr() {

}

std::unique_ptr<PrototypeAST> ListParser::parseExtern() {

}

std::unique_ptr<ExprAST> ListParser::parseIfExpr() {

}

std::unique_ptr<ExprAST> ListParser::parseForExpr() {

}

