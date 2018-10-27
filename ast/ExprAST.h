#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "llvm/IR/BasicBlock.h"

class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual llvm::Value *codegen() = 0;
};

#endif
