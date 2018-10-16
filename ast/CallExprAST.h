#ifndef __CALL_EXPR_AST_H__
#define __CALL_EXPR_AST_H__

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"

class CallExprAST : public ExprAst {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args):
        Callee(Callee), Args(Args) 
    {}

    llvm::Value *codegen() override;
}

#endif
