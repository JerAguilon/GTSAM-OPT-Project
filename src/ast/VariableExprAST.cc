#include "ast/VariableExprAST.h"
#include "logger/logger.h"
#include "kaleidoscope/kaleidoscope.h"

#include <iostream>
Value *VariableExprAST::codegen() {
    // Look this variable up in the function.
    Value *V = NamedValues[Name];
    if (!V)
        return LogErrorV(("Unknown variable name: " + Name).c_str());
    std::cout << "TYPE: " << V->getType() << std::endl;
    return Builder.CreateLoad(V, Name.c_str());
}

