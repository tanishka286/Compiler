#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "autofix.h"

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
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("HASC Compiler - Habit-Aware Adaptive Compiler\n");
        printf("Usage:\n");
        printf("  hasc <source_file>     Compile and analyze source file\n");
        printf("  hasc --reset           Reset habit detection history\n");
        printf("  hasc --help            Show this help message\n");
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "--reset") == 0) {
        autofix_reset_count();
        autofix_reset_lines();

        if (remove("data/user_profile.dat") == 0) {
            printf("Habit history reset successfully.\n");
        } else {
            printf("No habit history found to reset.\n");
        }
        return 0;
    }

    if (argc == 1) {
        fprintf(stderr, "Usage: hasc <source_file> | --reset | --help\n");
        return 1;
    }

    if (argc != 2) {
        fprintf(stderr, "Usage: hasc <source_file> | --reset | --help\n");
        return 1;
    }

    init_lexer(argv[1]);
    autofix_reset_count();
    autofix_reset_lines();

    parser_init();
    parse_program();
    parser_close();

    close_lexer();
    return 0;
}
