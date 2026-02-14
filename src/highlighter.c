// Code highlighting

#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "highlighter.h"

static const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD:    return "TOKEN_KEYWORD";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER:     return "TOKEN_NUMBER";
        case TOKEN_SYMBOL:     return "TOKEN_SYMBOL";
        case TOKEN_EOF:        return "TOKEN_EOF";
        case TOKEN_UNKNOWN:    return "TOKEN_UNKNOWN";
        default:               return "UNKNOWN";
    }
}

void highlight_error(const char *error_type,
                     const char *expected_type,
                     const char *expected_lexeme,
                     const Token *actual_token) {
    printf("\n");
    printf("Line %d, Column %d:\n", actual_token->line, actual_token->column);
    
    /* Print caret indicator */
    int i;
    for (i = 0; i < actual_token->column; i++) {
        printf(" ");
    }
    printf("^\n");
    
    /* Print explanation based on error type */
    if (expected_type != NULL && strcmp(expected_type, "TOKEN_SYMBOL") == 0 &&
        expected_lexeme != NULL && strcmp(expected_lexeme, ";") == 0) {
        /* Missing semicolon error */
        printf("Missing semicolon at end of statement.\n");
        printf("Statements must end with ';'.\n");
        printf("Example: <statement>;\n");
    } else {
        /* Generic syntax error explanation */
        printf("What went wrong: ");
        if (expected_type != NULL && expected_lexeme != NULL) {
            printf("Expected %s \"%s\"", expected_type, expected_lexeme);
        }
        if (actual_token != NULL) {
            printf(", but found %s \"%s\"", 
                   token_type_to_string(actual_token->type),
                   actual_token->lexeme);
        }
        printf(".\n");
    }
    
    printf("\n");
}
