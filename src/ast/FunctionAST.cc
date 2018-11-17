#include "ast/FunctionAST.h"
#include "logger/logger.h"
#include "kaleidoscope/kaleidoscope.h"
#include "utils/functions.h"

using namespace llvm;

Function *FunctionAST::codegen() {
    // Transfer ownership of the prototype to the FunctionProtos map, but keep a
    // reference to it for use below.
    auto &P = *Proto;
    FunctionProtos[Proto->getName()] = std::move(Proto);
    Function *TheFunction = getFunction(P.getName());
    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args()) {
        AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());

        Builder.CreateStore(&Arg, Alloca);
        NamedValues[Arg.getName()] = Alloca;
    }

    Value *RetVal = Body->codegen();
    // Finish off the function.

    if (RetVal) {
        Builder.CreateRet(RetVal);
    } else {
        Builder.CreateRetVoid();
    }

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    // Run the optimizer on the function.
    TheFPM->run(*TheFunction);

    return TheFunction;
}
