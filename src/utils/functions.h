#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ast/PrototypeAST.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "include/KaleidoscopeJIT.h"

// TODO(jeremy): Create an instance that moves this out of global state
extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
extern std::unique_ptr<legacy::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;

Function *getFunction(std::string Name);
AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type* type=nullptr);

#endif
