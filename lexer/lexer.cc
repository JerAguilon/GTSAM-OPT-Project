#include<vector>
#include<iostream>

#include  "lexer/lexer.h"


namespace lexer {   

    Token& make_token(
        std::vector<Token>& tokens, const char *filename, TokenType tok_type, int line, int col
    ) {
        tokens.emplace_back();
        Token &tok = tokens.back();
        tok.type = tok_type;
        tok.filename = (char *) filename;
        tok.line_number = line;
        tok.column_number = col;
        return tok;
    }

    int tokenize_stream(std::istream& file, const char*fname, std::vector<Token>& tokens) {
        std::string line;
        int line_number = 0;

        while (std::getline(file, line)) {
            line_number++; 
            int i = 0;
            bool in_line_comment = false;

            while (i < line.size()) {
                char ch = line[i];
                if (std::isspace(ch)) {
                    i++;
                    continue;
                }

                // comment
                if (ch == '/' && line[i + 1] == '/') {
                    i += 2;
                    in_line_comment = true;
                    continue;
                }

                if (in_line_comment) {
                    i++; //ignore comments
                }


                // identifier or keyword
                if ((!std::ispunct(ch) && !std::isdigit(ch)) ||  ch == '_') {
                    Token& tok = make_token(tokens, fname, TokenType::tok_type_name, line_number, i + 1);
                    int name_start_index = i;

                    while(true) {
                        i++;
                        if (i >= line.size()) break;

                        char name_ch = line[i];
                        if (std::isspace(name_ch)) break;
                        if (name_ch == '_' || !std::ispunct(name_ch)) continue;
                        break;
                    }
                    int name_len = i - name_start_index;
                    tok.str_content = line.substr(name_start_index, name_len);
                }
            }

            // TODO: numbers

        }
    }
}
