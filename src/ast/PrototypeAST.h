#ifndef PROTOTYPE_AST_H
#define PROTOTYPE_AST_H

#include "kaleidoscope/kaleidoscope.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    Type* ReturnType;
    std::vector<llvm::Type *> ArgTypes;

    public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args, Type* ReturnType=Type::getDoubleTy(TheContext), std::vector<llvm::Type*> ArgTypes={})
        : Name(Name), Args(std::move(Args)), ReturnType(ReturnType), ArgTypes(ArgTypes) {}

    Function *codegen();
    const std::string &getName() const { return Name; }
};

#endif
