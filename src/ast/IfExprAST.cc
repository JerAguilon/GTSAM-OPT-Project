#include "llvm/IR/Function.h"

#include "ast/IfExprAST.h"
#include "kaleidoscope/kaleidoscope.h"
#include "logger/logger.h"
#include "utils/functions.h"
#include "ast/ExprAST.h"
#include "logger/logger.h"

Value *IfExprAST::codegen() {
    Value *CondV = Cond->codegen();
    if (!CondV) return nullptr;

    CondV = Builder.CreateFCmpONE(
        CondV, ConstantFP::get(TheContext, APFloat(0.0)), "ifcond"
    );
    Function *TheFunction = Builder.GetInsertBlock()->getParent();

    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
    BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // then block
    Builder.SetInsertPoint(ThenBB);

    Value *ThenV = Then->codegen();

    if (!ThenV) return nullptr;

    Builder.CreateBr(MergeBB);

    // Codegen of then can change the current block, so update ThenBB for the PHI
    ThenBB = Builder.GetInsertBlock();
    
    // else block
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    Value *ElseV = Else->codegen();
    if (!ElseV) return nullptr;

    Builder.CreateBr(MergeBB);
    // Codegen of else can change the curren tblock, so update ElseBB for the PHI
    ElseBB = Builder.GetInsertBlock();

    // Merge block
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    PHINode *PN = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, "iftmp");

    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    return PN;
}
