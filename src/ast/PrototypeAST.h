#ifndef PROTOTYPE_AST_H
#define PROTOTYPE_AST_H

#include "kaleidoscope/kaleidoscope.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    Type* ReturnType;

    public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args, Type* ReturnType=Type::getDoubleTy(TheContext))
        : Name(Name), Args(std::move(Args)), ReturnType(ReturnType) {}

    Function *codegen();
    const std::string &getName() const { return Name; }
};

#endif
