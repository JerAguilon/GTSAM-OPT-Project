#ifndef PROTOTYPE_AST_H
#define PROTOTYPE_AST_H

#include "llvm/IR/IRBuilder.h"
#include "utils/utils.h"

using namespace llvm;

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

    public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}

    Function *codegen();
    const std::string &getName() const { return Name; }
};

#endif
