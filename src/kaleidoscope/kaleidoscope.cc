#include "kaleidoscope.h"

void registerPose2Struct() {
    llvm::Type* doubleType = llvm::Type::getDoubleTy(TheContext);
    llvm::StructType *structType = llvm::StructType::create(
        TheContext,
        {doubleType, doubleType, doubleType},
        "Pose2d",
        false
    );
    pose2Type = structType;
}


// This is an object that owns LLVM core data structures
llvm::LLVMContext TheContext;

// This is a helper object that makes easy to generate LLVM instructions
llvm::IRBuilder<> Builder(TheContext);

// This is an LLVM construct that contains functions and global variables
std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
std::map<std::string, llvm::AllocaInst *> NamedValues;

// This is a native data type: a 2d pose
llvm::StructType *pose2Type;

