#include "ast/NumberExprAST.h"
#include "utils/utils.h"

llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(TheContext, APFloat(Val));
}
