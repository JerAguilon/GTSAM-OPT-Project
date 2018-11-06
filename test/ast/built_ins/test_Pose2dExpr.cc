#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "ast/built_ins/Pose2dExprAST.h"
#include "ast/NumberExprAST.h"
#include "kaleidoscope/kaleidoscope.h"

BOOST_AUTO_TEST_SUITE( TestPose2dExpr )

BOOST_AUTO_TEST_CASE(TestingPose)
{
    registerPose2Struct();
    std::cout << "RESULT TEST: "  << std::endl;
    auto test = NumberExprAST(3);
    auto test_result = test.codegen();
    test_result->print(llvm::errs());
    fprintf(stderr, "\n");
    Pose2dExprAST pose(2, 3, 4, "tmp_pose");

    llvm::Value* result = pose.codegen();
    std::cout << result << std::endl;
    result->print(llvm::errs());
    fprintf(stderr, "\n");

    /* BOOST_CHECK_EQUAL(tokens.size(), expected.size()); */
    /* for (int i = 0; i < expected.size(); i++) { */
    /*     BOOST_CHECK(tokens[i] == expected[i]); */
    /* } */

}


BOOST_AUTO_TEST_SUITE_END()
