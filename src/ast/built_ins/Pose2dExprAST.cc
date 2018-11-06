#include "ast/built_ins/Pose2dExprAST.h"
#include "built_ins/BuiltInTypes.h"
#include <iostream>

llvm::Value *Pose2dExprAST::codegen() {
    auto ptr_x = llvm::ConstantFP::get(TheContext, llvm::APFloat(x));
    auto ptr_y = llvm::ConstantFP::get(TheContext, llvm::APFloat(y));
    auto ptr_theta = llvm::ConstantFP::get(TheContext, llvm::APFloat(theta));

    std::cout << (pose2Type == nullptr) << std::endl;
    llvm::AllocaInst* instance = Builder.CreateAlloca(
        pose2Type, nullptr, name
    );
    std::cout << "YAS COMPLETED" << std::endl;
    llvm::Value* gep_x = Builder.CreateInBoundsGEP(
        pose2Type, 
        instance,
        {0, 0}, // index 0, 0: Pose2.x
        "gep_pose2_x"
    );
    Builder.CreateStore(ptr_x, gep_x);

    llvm::Value* gep_y = Builder.CreateInBoundsGEP(
        pose2Type, 
        instance,
        {0, 1}, // index 0, 0: Pose2.y
        "gep_pose2_y"
    );
    Builder.CreateStore(ptr_y, gep_y);

    llvm::Value* gep_theta = Builder.CreateInBoundsGEP(
        pose2Type, 
        instance,
        {0, 2}, // index 0, 0: Pose2.theta
        "gep_pose2_theta"
    );
    return Builder.CreateStore(ptr_theta, gep_theta);

}
