#include "parser/parser.h"
#include "logger/logger.h"
#include "utils/operators.h"

#include<string>

// Called when the current token is a tok_number
std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();

    auto V = ParseExpression();

    if (!V) return nullptr;

    if (CurTok != tok_rparen) return LogError("Expected )");

    getNextToken();
    return V;
}

std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken();

    if (CurTok != tok_lparen) return llvm::make_unique<VariableExprAST>(IdName);

    getNextToken();

    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != tok_rparen) {
        while (true) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }
            if (CurTok == tok_rparen) {
                break;
            }

            if (CurTok != tok_comma) {
                return LogError("Expected ')' or ',' in the argument list");
            }

            getNextToken();
        }
    }
    getNextToken();
    return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> ParsePrimary() {
    std::string default_error = "Unknown token when expecting an expression: ";
    default_error.push_back(CurTok);

    switch (CurTok) {
        default: return LogError(default_error.c_str());
        case tok_identifier: return ParseIdentifierExpr();
        case tok_number: return ParseNumberExpr();
        case tok_lparen: return ParseParenExpr();
        case tok_if: return ParseIfExpr();
    }
}

std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = getTokenPrecedence(CurTok);

        if (TokPrec < ExprPrec) {
            return LHS;
        }

        int BinOp = CurTok;
        getNextToken();

        auto RHS = ParsePrimary();
        if (!RHS) return nullptr;

        int NextPrec = getTokenPrecedence(CurTok);
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }
        LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();

    if (!LHS) return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier) {
        return LogErrorP("Expected function name in prototype");
    }

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != tok_lparen) {
        return LogErrorP("Expected '(' in prototype");
    }

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier) {
        ArgNames.push_back(IdentifierStr);
    }

    if (CurTok != tok_rparen) {
        return LogErrorP("Expected ')' in prototype");
    }

    getNextToken();
    return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}
std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken();

    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression()) {
        return llvm:: make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
}
std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto = llvm::make_unique<PrototypeAST>("anon_expr", std::vector<std::string>());
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
}

std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

static std::unique_ptr<ExprAST> ParseIfExpr() {
    getNextToken(); // eat the if

    auto Cond = ParseExpression();
    if (!Cond) return nullptr;

    if (CurTok != tok_then) return LogError("expected then");
    getNextToken(); // eat the then

    auto Then = ParseExpression();
    if (!Then) return nullptr;

    if (CurTok != tok_else) return LogError("Expected else");

    getNextToken();

    auto Else = ParseExpression();
    if  (!Else) return nullptr;
    
    return llvm::make_unique<IfExprAST>(std::move(Cond), std::move(Then), std::move(Else));
}
