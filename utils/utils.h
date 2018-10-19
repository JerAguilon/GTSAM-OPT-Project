#ifndef UTILS_H
#define UTILS_H

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

#include "ast/PrototypeAST.h"
#include "KaleidoscopeJIT.h" //TODO(jeremy): Refactor the JIT

using namespace llvm;
using namespace llvm::orc;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;
extern std::unique_ptr<Module> TheModule;
extern std::map<std::string, Value *> NamedValues;
extern std::unique_ptr<legacy::FunctionPassManager> TheFPM;
extern std::unique_ptr<KaleidoscopeJIT> TheJIT;


#endif

