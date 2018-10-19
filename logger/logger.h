#ifndef LOGGER_H
#define LOGGER_H

#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
llvm::Value *LogErrorV(const char *Str);

#endif
