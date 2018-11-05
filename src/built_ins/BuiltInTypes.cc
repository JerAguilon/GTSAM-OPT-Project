#include <vector>

#include "built_ins/BuiltInTypes.h"

llvm::StructType *pose2Type = registerPose2Struct();

static llvm::StructType *registerPose2Struct() {
    llvm::Type* doubleType = llvm::Type::getFloatTy(TheContext);
    llvm::StructType *structType = llvm::StructType::create("Pose2d", doubleType, doubleType, doubleType);
    return structType;
}
