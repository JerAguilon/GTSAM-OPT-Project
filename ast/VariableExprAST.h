#ifndef __VARIABLE_EXPR_AST_H__
#define __VARIABLE_EXPR_AST_H__

#include "ast/ExprAST.h"

class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &Name): Name(Name) {}
    llvm::Value *codegen() override;
};

#endif
