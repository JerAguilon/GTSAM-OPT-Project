#ifndef __BINARY_EXPR_AST_H__
#define __BINARY_EXPR_AST_H__

#include "llvm/IR/IRBuilder.h"
#include "ast/ExprAST.h"

class BinaryExprAST : pulbic ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS std::unique_ptr<ExprAST> RHS) :
            Op(op),
            LHS(LHS),
            RHS(RHS)
        {}

        llvm::Value *codegen() override;
};

#endif
