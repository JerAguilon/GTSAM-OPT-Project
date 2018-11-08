#include "ast/PrototypeAST.h"

using namespace llvm;

Function *PrototypeAST::codegen() {
    // Make the function type:  double(double,double) etc.
    FunctionType *FT;
    if (ArgTypes.size() == 0) {
        std::vector<Type *> arg_types(Args.size(), Type::getDoubleTy(TheContext));
        FT = FunctionType::get(ReturnType, arg_types, false);
    } else {
        FT = FunctionType::get(ReturnType, ArgTypes, false);
    }

    Function *F =
        Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;
}
