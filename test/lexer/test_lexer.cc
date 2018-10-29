#define BOOST_TEST_MODULE SqrTests
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "lexer/lexer.h"
#include "lexer/token.h"

bool operator==(const TokenWrapper a, const TokenWrapper b) {
    return (
        (a.type == b.type) &&
        (a.value == b.value) &&
        (a.str_content == b.str_content) &&
        (strcmp(a.filename, b.filename) == 0) &&
        (a.line_number == b.line_number) &&
        (a.column_number == b.column_number)
    );
}


BOOST_AUTO_TEST_CASE(PassTest)
{
    std::string line = "1 < 3;";
    std::istringstream stream(line);
    std::vector<TokenWrapper> tokens;
    std::string fname = "foobar.txt";

    TokenWrapper e1 = {};
    e1.value = 1;
    e1.filename = (char *) "foobar.txt";
    e1.type = tok_number;
    e1.line_number = 1;
    e1.column_number = 1;

    TokenWrapper e2 = {};
    e2.filename = (char *) "foobar.txt";
    e2.type = '<';
    e2.line_number = 1;
    e2.column_number = 3;

    TokenWrapper e3 = {};
    e3.value = 3;
    e3.filename = (char *) "foobar.txt";
    e3.type = tok_number;
    e3.line_number = 1;
    e3.column_number = 5;

    std::vector<TokenWrapper> expected {e1, e2, e3};
    tokenizeStream(stream, fname.c_str(), tokens);

    BOOST_CHECK_EQUAL(tokens.size(), expected.size());
    for (int i = 0; i < expected.size(); i++) {
        BOOST_CHECK(tokens[i] == expected[i]);
    }
}
