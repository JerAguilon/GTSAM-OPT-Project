#include "ast/NumberExprAST.h"
#include "kaleidoscope/kaleidoscope.h"

#include "llvm/ADT/APFloat.h"

using namespace llvm;

llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(TheContext, APFloat(Val));
}
