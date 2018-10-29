#include "ast/BinaryExprAST.h"
#include "kaleidoscope/kaleidoscope.h"
#include "lexer/token.h"

using namespace llvm;

Value *BinaryExprAST::codegen() {
    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    if (!L || !R)
        return nullptr;

    switch (Op) {
        case tok_add:
            return Builder.CreateFAdd(L, R, "addtmp");
        case tok_sub:
            return Builder.CreateFSub(L, R, "subtmp");
        case tok_mul:
            return Builder.CreateFMul(L, R, "multmp");
        case tok_div:
            return Builder.CreateFDiv(L, R, "multmp");
        case tok_less:
            L = Builder.CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        case tok_greater:
            L = Builder.CreateFCmpULT(R, L, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        case tok_lessequal:
            L = Builder.CreateFCmpULE(L, R, "cmptmp");
            return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        case tok_greatequal:
            L = Builder.CreateFCmpULE(R, L, "cmptmp");
            return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        default:
            return LogErrorV("invalid binary operator");
    }
}
