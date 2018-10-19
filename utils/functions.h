#include  "ast/PrototypeAST.h"

// TODO(jeremy): Create an instance that moves this out of global state
extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
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
