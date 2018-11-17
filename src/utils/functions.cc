#include "utils/functions.h"
#include "kaleidoscope/kaleidoscope.h"
#include "include/KaleidoscopeJIT.h"

std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
std::unique_ptr<legacy::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;


Function *getFunction(std::string Name) {
    // First, see if the function has already been added to the current module.
    if (auto *F = TheModule->getFunction(Name))
        return F;

    // If not, check whether we can codegen the declaration from some existing
    // prototype.
    auto FI = FunctionProtos.find(Name);
    if (FI != FunctionProtos.end())
        return FI->second->codegen();

    // If no existing prototype exists, return null.
    return nullptr;
}

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type* type) {
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
            TheFunction->getEntryBlock().begin());
    if (!type) {
        return TmpB.CreateAlloca(Type::getDoubleTy(TheContext), 0, VarName.c_str());
    } else {
        return TmpB.CreateAlloca(type, 0, VarName.c_str());
    }
}
