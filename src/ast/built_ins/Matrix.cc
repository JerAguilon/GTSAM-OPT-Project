#include "ast/built_ins/Matrix.h"
#include "llvm/ADT/APFloat.h"


llvm::Value *Matrix::codegen() {
    auto rows = values.size();
    auto cols = values[0].size();
    
    llvm::AllocaInst* instance = (llvm::AllocaInst*) destination->codegen();

    for (int row = 0; row < rows; row++ ) {
        for (int col = 0; col < cols; col++) {
            auto rowValue = llvm::Constant::getIntegerValue(
                llvm::Type::getInt32Ty(TheContext),        
                llvm::APInt(32, row)
            );
            auto colValue = llvm::Constant::getIntegerValue(
                llvm::Type::getInt32Ty(TheContext),        
                llvm::APInt(32, col)
            );
            std::vector<llvm::Value *> position {rowValue, colValue};        
            llvm::Value* elem_ptr = Builder.CreateGEP(
                instance,
                position
            );
            auto value_to_insert = llvm::ConstantFP::get(TheContext, llvm::APFloat(values[row][col]));
            Builder.CreateStore(value_to_insert, elem_ptr);
        }
    }
    return instance;
}

