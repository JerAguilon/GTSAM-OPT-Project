#ifndef POSE_2d_EXPR_H
#define POSE_2d_EXPR_H

#include<string>

#include "kaleidoscope/kaleidoscope.h"
#include "ast/CallExprAST.h"
#include "llvm/IR/IRBuilder.h"

class Pose2dExpr : ExprAST{
    double x, y, theta;  
    std::string name;

public:
    Pose2dExpr(double x, double y, double theta, std::string name) :
        x(x), y(y), theta(theta), name(name)
    {}
    llvm::Value *codegen() override;
};

#endif
