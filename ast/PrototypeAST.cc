/* #ifndef PROTOTYPE_H */
/* #define PROTOTYPE_H */

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args) :
        Name(name), Args(Args)
    {}

    llvm::Function *codegen();

    const std::string &getName() const {
        return Name;
    }
};

/* #endif */
