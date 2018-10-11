#include<string>
#include<vector>
#include<map>
#include<llvm/IR/Module.h>
#include<llvm/IR/Value.h>
#include<llvm/IR/IRBuilder.h>
#include<iostream>

using namespace llvm;

class ExprAST {
    public:
        virtual ~ExprAST() {}
        virtual Value *Codegen() = 0;
};

class NumberExprAST : public ExprAST {
    double Val;

    public:
        NumberExprAST(double val) : Val(val) {}
        Value *Codegen();
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

    public:
        VariableExprAST(const std::string &name) : Name(name) {}
        Value *Codegen();
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;

    public:
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
            : Op(op), LHS(lhs), RHS(rhs) {}
        Value *Codegen();
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST*> Args;
    public:
        CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
            : Callee(callee), Args(args) {}
        Value *Codegen();
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its argument names as well as if it is an operator.
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    std::vector<Type *> FormalTypes;
    Type *ReturnType;
    bool isOperator;
    unsigned Precedence;  // Precedence if a binary op.
    public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args,
            const std::vector<Type *> &formals, Type *ret,
            bool isoperator = false, unsigned prec = 0)
        : Name(name), Args(args), FormalTypes(formals), ReturnType(ret), 
        isOperator(isoperator), Precedence(prec) {}

    bool isUnaryOp() const { return isOperator && Args.size() == 1; }
    bool isBinaryOp() const { return isOperator && Args.size() == 2; }

    char getOperatorName() const {
        assert(isUnaryOp() || isBinaryOp());
        return Name[Name.size()-1];
    }

    unsigned getBinaryPrecedence() const { return Precedence; }

    Function *Codegen();

    void CreateArgumentAllocas(Function *F);

    virtual Type *getType() const { return ReturnType; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  PrototypeAST *Proto;
  ExprAST *Body;
public:
  FunctionAST(PrototypeAST *proto, ExprAST *body)
    : Proto(proto), Body(body) {}

  Function *Codegen();

  virtual Type *getType() const { return Proto->getType(); }
};


ExprAST *MyError(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0; }
Value *ErrorV(const char *Str) { MyError(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { MyError(Str); return 0; }

static Module *TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
std::map<std::string, AllocaInst*> NamedValues;

Value *NumberExprAST::Codegen() {
  return ConstantFP::get(TheContext, APFloat(Val));
}

Value *VariableExprAST::Codegen() {
  // Look this variable up in the function.
  Value *V = NamedValues[Name];
  return V ? V : ErrorV("Unknown variable name");
}

Value *BinaryExprAST::Codegen() {
  Value *L = LHS->Codegen();
  Value *R = RHS->Codegen();
  if (L == 0 || R == 0) return 0;

  switch (Op) {
  case '+': return Builder.CreateFAdd(L, R, "addtmp");
  case '-': return Builder.CreateFSub(L, R, "subtmp");
  case '*': return Builder.CreateFMul(L, R, "multmp");
  case '<':
    L = Builder.CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext),
                                "booltmp");
  default: return ErrorV("invalid binary operator");
  }
}

Value *CallExprAST::Codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = TheModule->getFunction(Callee);
  if (CalleeF == 0)
    return ErrorV("Unknown function referenced");

  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return ErrorV("Incorrect # arguments passed");

  std::vector<Value*> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->Codegen());
    if (ArgsV.back() == 0) return 0;
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::Codegen() {
  FunctionType *FT = FunctionType::get(ReturnType, FormalTypes, false);

  Function *F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule);

  // If F conflicted, there was already something named 'Name'.  If it has a
  // body, don't allow redefinition or reextern.
  if (F->getName() != Name) {
    // Delete the one we just made and get the existing one.
    F->eraseFromParent();
    F = TheModule->getFunction(Name);

    // If F already has a body, reject this.
    if (!F->empty()) {
      ErrorF("redefinition of function");
      return 0;
    }

    // If F took a different number of args, reject.
    if (F->arg_size() != Args.size()) {
      ErrorF("redefinition of function with different # args");
      return 0;
    }
  }

  // Set names for all arguments.
  unsigned Idx = 0;
  for (Function::arg_iterator AI = F->arg_begin(); Idx != Args.size();
       ++AI, ++Idx)
    AI->setName(Args[Idx]);

  return F;
}

int main() {
    std::cout << "FOOBAR";
}
