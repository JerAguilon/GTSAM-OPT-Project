#ifndef POSE_2d_EXPR_H
#define POSE_2d_EXPR_H

#include<string>

#include "kaleidoscope/kaleidoscope.h"
#include "ast/ExprAST.h"
#include "ast/VariableExprAST.h"
#include "llvm/IR/IRBuilder.h"

class Pose2dExprAST : public ExprAST {
    double x, y, theta;  
    std::string name;
    std::unique_ptr<VariableExprAST> destination;

public:
    Pose2dExprAST(double x, double y, double theta, std::unique_ptr<VariableExprAST> destination) :
        x(x), y(y), theta(theta), destination(std::move(destination))
    {
    }
    llvm::Value *codegen() override;
};

#endif
