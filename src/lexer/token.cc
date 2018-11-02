#include "lexer/token.h"

#include <boost/format.hpp>

std::string TokenWrapper::to_string() {
    boost::format fmt = boost::format("Type: %1%, Content: %2%, Value: %3%")
            % type % str_content % value;
    return fmt.str();
}
