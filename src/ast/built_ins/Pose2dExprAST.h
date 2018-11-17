#ifndef POSE_2d_EXPR_H
#define POSE_2d_EXPR_H

#include<string>

#include "kaleidoscope/kaleidoscope.h"
#include "ast/ExprAST.h"
#include "ast/VariableExprAST.h"
#include "llvm/IR/IRBuilder.h"

class Pose2dExprAST : public ExprAST {
    std::unique_ptr<ExprAST> x, y, theta;  
    std::string name;
    std::unique_ptr<VariableExprAST> destination;

public:
    Pose2dExprAST(
        std::unique_ptr<ExprAST> x,
        std::unique_ptr<ExprAST> y,
        std::unique_ptr<ExprAST> theta,
        std::unique_ptr<VariableExprAST> destination
    ) :
        x(std::move(x)), y(std::move(y)), theta(std::move(theta)), destination(std::move(destination))
    {
    }
    llvm::Value *codegen() override;
};

#endif
