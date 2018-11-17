#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "llvm/Support/TargetSelect.h"

#include "ast/FunctionAST.h"
#include "ast/VariableExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/built_ins/Pose2dExprAST.h"
#include "include/KaleidoscopeJIT.h"
#include "kaleidoscope/environment.h"
#include "kaleidoscope/kaleidoscope.h"
#include "utils/functions.h"

#include <iostream>

using namespace llvm;
using namespace llvm::orc;

typedef void (*testfunc_t)(Pose2d*);

BOOST_AUTO_TEST_SUITE( TestPose2dExpr )

BOOST_AUTO_TEST_CASE(TestingPose)
{

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    TheJIT = llvm::make_unique<KaleidoscopeJIT>();
    InitializeModuleAndPassManager();

    registerPose2Struct();
    std::cout << "POSE2D TYPE: " << pose2Type << std::endl;

    auto destination = llvm::make_unique<VariableExprAST>("dst");
    auto x = llvm::make_unique<NumberExprAST>(2);
    auto y = llvm::make_unique<NumberExprAST>(3);
    auto theta = llvm::make_unique<NumberExprAST>(4);
    auto pose = llvm::make_unique<Pose2dExprAST>(std::move(x), std::move(y), std::move(theta), std::move(destination));
    auto proto = llvm::make_unique<PrototypeAST>(
            "tmp_proto",
            std::vector<std::string>{"dst"},
            llvm::Type::getVoidTy(TheContext),
            std::vector<llvm::Type*>{llvm::PointerType::get(pose2Type, 0)}
    );
    auto function = llvm::make_unique<FunctionAST>(std::move(proto), std::move(pose));

    llvm::Value* result = function->codegen();
    result->print(llvm::errs());
    fprintf(stderr, "\n");

    auto H = TheJIT->addModule(std::move(TheModule));
    InitializeModuleAndPassManager();

    auto ExprSymbol = TheJIT->findSymbol("tmp_proto");
    assert(ExprSymbol && "Function not found");

    // Get the symbol's address and cast it to the right type (takes no
    // arguments, returns a double) so we can call it as a native function.
    testfunc_t FP = (testfunc_t) cantFail(ExprSymbol.getAddress());

    Pose2d my_pose = {0,0,0};
    auto ptr = &my_pose;
    FP(ptr);

    BOOST_CHECK_EQUAL(ptr->x, 2);
    BOOST_CHECK_EQUAL(ptr->y, 3);
    BOOST_CHECK_EQUAL(ptr->theta, 4);
    BOOST_CHECK_EQUAL(my_pose.x, 2);
    BOOST_CHECK_EQUAL(my_pose.y, 3);
    BOOST_CHECK_EQUAL(my_pose.theta, 4);

    // Delete the anonymous expression module from the JIT.
    TheJIT->removeModule(H);
}


BOOST_AUTO_TEST_SUITE_END()
