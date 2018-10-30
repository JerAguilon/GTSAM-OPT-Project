#include "lexer/lexer.h"
#include "lexer/token.h"
#include "logger/logger.h"

#include <iostream>


Token CurTok;
std::string IdentifierStr; // Filled in if tok_identifier
double NumVal;             // Filled in if tok_number

static bool iscmp(int c) {
    return c == '<' || c == '>';
}

/// gettok - Return the next token from standard input.
Token gettok() {
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar)) {
        LastChar = getchar();
    }

    // Test for <= or >=
    if (iscmp(LastChar)) {
        IdentifierStr = LastChar;
        LastChar = getchar();
        if (LastChar == '=')
            IdentifierStr += LastChar;
            LastChar = getchar();

        if (IdentifierStr == ">=")
            return tok_greatequal;
        if (IdentifierStr == "<=")
            return tok_lessequal;
        if (IdentifierStr ==  "<")
            return tok_less;
        if (IdentifierStr ==  ">")
            return tok_greater;
    }

    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        if (IdentifierStr == "if")
            return tok_if;
        if (IdentifierStr == "then")
            return tok_then;
        if (IdentifierStr == "else")
            return tok_else;
        if (IdentifierStr == "for")
            return tok_for;
        if (IdentifierStr == "in")
            return tok_in;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    char ThisChar = LastChar;

    Token t;
    switch(ThisChar) {
        case '+':
            t = tok_add;
            break;
        case '-':
            t = tok_sub;
            break;
        case '/':
            t = tok_div;
            break;
        case '*':
            t = tok_mul;
            break;
        case ';':
            t = tok_semicolon;
            break;
        case '(':
            t = tok_lparen;
            break;
        case ')':
            t = tok_rparen;
            break;
        case ',':
            t = tok_comma;
            break;
        default:
            LogError("Unknown token encountered");
            break;
    }

    LastChar = getchar();
    return t;
}

Token getNextToken() {
    return CurTok = gettok();
}


static TokenWrapper& build_token(
        std::vector<TokenWrapper>& tokens, const char *filename, Token tok_type, int line_number, int column_number) {
    tokens.emplace_back();
    TokenWrapper &tok = tokens.back();

    tok.type = tok_type;
    tok.filename = (char *) filename;
    tok.line_number = line_number;
    tok.column_number = column_number;
    return tok;
}

std::vector<TokenWrapper>& tokenizeStream(std::istream& infile, const char* fname, std::vector<TokenWrapper>& tokens) {
    std::string line; 
    int line_number = 0;


    while (std::getline(infile, line)) {
        line_number++;
        int i = 0;

        while (i < line.size()) {
            char ch = line[i];
            if (std::isspace(ch)) {
                i++;
                continue;
            }
            if (ch == '#') {
                break; // line comments go until the end of the line
            }

            // Test for <= or >=
            if (iscmp(ch)) {
                Token identifier; // Temporary hack
                std::string symbol;
                symbol.push_back(ch);
                if (i + 1 < line.size() && line[i + 1] == '=') {
                    i++;
                    symbol.push_back(line[i]); 
                }
                if (symbol == ">=") identifier = tok_greatequal;
                if (symbol == "<=") identifier = tok_lessequal;
                if (symbol ==  "<") identifier = tok_less;
                if (symbol ==  ">") identifier = tok_greater;

                build_token(tokens, fname, identifier, line_number, i + 1);
                i++;
                continue;
            }

            // identifier: [a-zA-Z][a-zA-Z0-9]*
            if (isalpha(ch)) { 
                std::string symbol;
                symbol.push_back(ch);
                while (i + 1 < line.size() && isalnum(line[i + 1])) {
                    i++; 
                    symbol.push_back(line[i]);
                }

                Token token_type; // Temporary hack 
                if (symbol == "def") {
                    token_type = tok_def;
                } else if (symbol == "extern") {
                    token_type = tok_extern;
                } else if (symbol == "if") {
                    token_type = tok_if;
                } else if (symbol == "then") {
                    token_type = tok_then;
                } else if (symbol == "else") {
                    token_type = tok_else;
                } else if (symbol == "for") {
                    token_type = tok_for;
                } else if (symbol == "in") {
                    token_type = tok_in;
                } else {
                    token_type = tok_identifier;
                }
                TokenWrapper& tok = build_token(tokens, fname, token_type, line_number, i + 1);
                if (token_type == tok_identifier) {
                    tok.str_content = symbol;
                }
                i++;
                continue;
            }

            if (isdigit(ch) || ch == '.') { // Number: [0-9.]+
                std::string symbol;
                symbol.push_back(ch);
                while (i + 1 < line.size() && (isdigit(line[i + 1]) || ch == '.')) {
                    i++; 
                    symbol.push_back(line[i]);
                }
                TokenWrapper& tok = build_token(tokens, fname, tok_number, line_number, i + 1);
                float val = strtod(symbol.c_str(), nullptr);
                tok.value = val;
            }
            i++;
        }
    }


    return tokens;
}
