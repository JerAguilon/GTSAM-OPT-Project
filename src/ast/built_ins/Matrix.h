#ifndef MATRIX_H
#define MATRIX_H

#include<string>

#include "kaleidoscope/kaleidoscope.h"
#include "ast/ExprAST.h"
#include "ast/VariableExprAST.h"
#include "llvm/IR/IRBuilder.h"

class Matrix : public ExprAST {
    std::vector<std::vector<double>> values;
    std::unique_ptr<VariableExprAST> destination;

public:
    Matrix(
        std::vector<std::vector<double>>& values,
        std::unique_ptr<VariableExprAST> destination
    ) :
        values(values), destination(std::move(destination))
    {
    }
    llvm::Value *codegen() override;
};
#endif
