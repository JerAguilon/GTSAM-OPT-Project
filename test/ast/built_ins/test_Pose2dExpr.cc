#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "llvm/Support/TargetSelect.h"

#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/built_ins/Pose2dExprAST.h"
#include "include/KaleidoscopeJIT.h"
#include "kaleidoscope/environment.h"
#include "kaleidoscope/kaleidoscope.h"
#include "utils/functions.h"

using namespace llvm;
using namespace llvm::orc;

typedef Pose2d (*testfunc_t)();

BOOST_AUTO_TEST_SUITE( TestPose2dExpr )

BOOST_AUTO_TEST_CASE(TestingPose)
{

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    TheJIT = llvm::make_unique<KaleidoscopeJIT>();
    InitializeModuleAndPassManager();

    registerPose2Struct();

    auto pose = llvm::make_unique<Pose2dExprAST>(2, 3, 4, "tmp_pose");
    auto proto = llvm::make_unique<PrototypeAST>("tmp_proto", std::vector<std::string>{}, pose2Type);
    auto function = llvm::make_unique<FunctionAST>(std::move(proto), std::move(pose));

    llvm::Value* result = function->codegen();
    result->print(llvm::errs());
    fprintf(stderr, "\n");
    ///////////
      // JIT the module containing the anonymous expression, keeping a handle so
      // we can free it later.
      auto H = TheJIT->addModule(std::move(TheModule));
      InitializeModuleAndPassManager();

      // Search the JIT for the __anon_expr symbol.
      auto ExprSymbol = TheJIT->findSymbol("tmp_proto");
      assert(ExprSymbol && "Function not found");

      // Get the symbol's address and cast it to the right type (takes no
      // arguments, returns a double) so we can call it as a native function.
      testfunc_t FP = (testfunc_t) cantFail(ExprSymbol.getAddress());
      fprintf(stderr, "Evaluated to %f\n", FP().x);
      fprintf(stderr, "Evaluated to %f\n", FP().y);
      fprintf(stderr, "Evaluated to %f\n", FP().theta);

      // Delete the anonymous expression module from the JIT.
      TheJIT->removeModule(H);
    ///////////

    /* auto function_symbol = TheJIT->findSymbol("tmp_proto"); */
    /* testfunc_t the_function = (testfunc_t) cantFail(function_symbol.getAddress()); */

    /* auto initialized_object = the_function(); */
    /* fprintf(stderr, "Evaluated to %f\n", initialized_object); */
}


BOOST_AUTO_TEST_SUITE_END()
