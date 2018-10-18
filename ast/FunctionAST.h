#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "ast/PrototypeAST.h"

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : 
        Proto(std::move(Proto)), Body(std::move(Body))
    {}

    llvm::Function *codegen();
};

#endif
