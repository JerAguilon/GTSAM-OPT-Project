#include "lexer/token.h"

bool operator==(TokenWrapper a, TokenWrapper b) {
    return (
        a.type == b.type &&
        a.value == b.value &&
        a.str_content == b.str_content &&
        a.filename == b.filename &&
        a.line_number == b.line_number &&
        a.column_number == b.column_number
    );
}
