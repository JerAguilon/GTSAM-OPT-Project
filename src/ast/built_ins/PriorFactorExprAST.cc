#include "ast/built_ins/PriorFactorExprAST.h"
#include "built_ins/BuiltInTypes.h"

llvm::Value *PriorFactorExprAST::codegen() {

    auto ptr_prior = prior->codegen();
    auto ptr_noise_model = noise_model->codegen();
    llvm::AllocaInst* instance = (llvm::AllocaInst*) destination->codegen();

    llvm::APInt zero(32, 0);
    llvm::APInt one(32, 1);

    std::vector<llvm::Value *> prior_pos {
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(TheContext), zero),
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(TheContext), zero)
    };

    std::vector<llvm::Value *> noise_model_pos {
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(TheContext), zero),
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(TheContext), one)
    };

    llvm::Value* gep_x = Builder.CreateGEP(
        instance,
        prior_pos, // index 0, 0: PriorFactor.x
        "gep_prior_factor__prior"
    );
    Builder.CreateStore(ptr_prior, gep_x);

    llvm::Value* gep_y = Builder.CreateInBoundsGEP(
        instance,
        noise_model_pos, // index 0, 1: PriorFactor.noise_model
        "gep_prior_factor__noise_model"
    );
    Builder.CreateStore(ptr_noise_model, gep_y);

    return instance;
}
