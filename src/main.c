#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD:   return "TOKEN_KEYWORD";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER:    return "TOKEN_NUMBER";
        case TOKEN_SYMBOL:    return "TOKEN_SYMBOL";
        case TOKEN_EOF:       return "TOKEN_EOF";
        case TOKEN_UNKNOWN:   return "TOKEN_UNKNOWN";
        default:              return "UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    init_lexer(argv[1]);

    parser_init();
    parse_program();
    parser_close();

    close_lexer();
    return 0;
}
