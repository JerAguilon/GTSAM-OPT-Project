#ifndef BUILT_IN_TYPES_H
#define BUILT_IN_TYPES_H

#include "kaleidoscope/kaleidoscope.h"

#include "llvm/IR/DerivedTypes.h"

struct Pose2 {
    double x;
    double y;
    double theta;
};

extern llvm::StructType *pose2Type;

static llvm::StructType *registerPose2Struct();

#endif
