#ifndef PRIOR_FACTOR_EXPR_AST_H
#define PRIOR_FACTOR_EXPR_AST_H

#include<string>

#include "kaleidoscope/kaleidoscope.h"
#include "ast/ExprAST.h"
#include "ast/VariableExprAST.h"
#include "llvm/IR/IRBuilder.h"

class PriorFactorExprAST : public ExprAST {
    std::unique_ptr<ExprAST> prior;  
    std::unique_ptr<ExprAST> noise_model;  
    std::unique_ptr<VariableExprAST> destination;

public:
    PriorFactorExprAST(
        std::unique_ptr<ExprAST> prior,
        std::unique_ptr<ExprAST> noiseModel,
        std::unique_ptr<VariableExprAST> destination
    ) :
        prior(std::move(prior)), noise_model(std::move(noise_model)), destination(std::move(destination))
    {
    }
    llvm::Value *codegen() override;
};

#endif
