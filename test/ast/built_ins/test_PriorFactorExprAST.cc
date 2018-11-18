#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "llvm/Support/TargetSelect.h"

#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"
#include "ast/built_ins/Pose2dExprAST.h"
#include "ast/built_ins/PriorFactorExprAST.h"
#include "include/KaleidoscopeJIT.h"
#include "kaleidoscope/environment.h"
#include "kaleidoscope/kaleidoscope.h"
#include "utils/functions.h"

#include <iostream>

using namespace llvm;
using namespace llvm::orc;

typedef PriorFactor* (*testfunc_t)(Pose2d*, PriorFactor*);

BOOST_AUTO_TEST_SUITE( TestPriorFactorExprAST )

BOOST_AUTO_TEST_CASE(TestingPriorFactor)
{

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    TheJIT = llvm::make_unique<KaleidoscopeJIT>();
    InitializeModuleAndPassManager();

    registerPose2Struct();
    registerPriorFactorStruct();

    auto destination = llvm::make_unique<VariableExprAST>("prior_dst");
    auto prior = llvm::make_unique<VariableExprAST>("pose_ptr");
    auto noise_model = llvm::make_unique<NumberExprAST>(2);
    auto prior_factor = llvm::make_unique<PriorFactorExprAST>(
        std::move(prior), std::move(noise_model), std::move(destination)
    );
    auto proto = llvm::make_unique<PrototypeAST>(
            "tmp_proto",
            std::vector<std::string>{"pose_ptr", "prior_dst"},
            llvm::PointerType::get(priorFactorType, 0),
            std::vector<llvm::Type*>{
                llvm::PointerType::get(pose2Type, 0), llvm::PointerType::get(priorFactorType, 0)
            }
    );
    auto function = llvm::make_unique<FunctionAST>(std::move(proto), std::move(prior_factor));

    llvm::Value* result = function->codegen();
    result->print(llvm::errs());
    fprintf(stderr, "\n");

    auto H = TheJIT->addModule(std::move(TheModule));
    InitializeModuleAndPassManager();

    auto ExprSymbol = TheJIT->findSymbol("tmp_proto");
    assert(ExprSymbol && "Function not found");

    /* // Get the symbol's address and cast it to the right type (takes no */
    /* // arguments, returns a double) so we can call it as a native function. */
    testfunc_t FP = (testfunc_t) cantFail(ExprSymbol.getAddress());

    Pose2d my_pose = {2, 3, 4};
    auto pose_ptr = &my_pose;

    PriorFactor my_prior;
    auto prior_ptr = &my_prior;


    FP(pose_ptr, prior_ptr);

    BOOST_CHECK_EQUAL(prior_ptr->prior, pose_ptr);
    BOOST_CHECK_EQUAL(prior_ptr->noise_model, 2);
    BOOST_CHECK_EQUAL(prior_ptr->prior->x, 2);
    BOOST_CHECK_EQUAL(prior_ptr->prior->y, 3);
    BOOST_CHECK_EQUAL(prior_ptr->prior->theta, 4);


    /* // Delete the anonymous expression module from the JIT. */
    TheJIT->removeModule(H);
}


BOOST_AUTO_TEST_SUITE_END()
