// Error history (user profile)

#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "error_tracker.h"

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

void error_tracker_log(const char *error_type,
                       const char *expected_type,
                       const char *expected_lexeme,
                       const Token *actual_token) {
    FILE *profile = fopen("data/user_profile.dat", "a");
    if (profile == NULL) {
        /* Silently fail if file cannot be opened */
        return;
    }

    fprintf(profile, "%s|%s|%s|%s|%s|%d|%d\n",
            error_type,
            expected_type ? expected_type : "",
            expected_lexeme ? expected_lexeme : "",
            token_type_to_string(actual_token->type),
            actual_token->lexeme,
            actual_token->line,
            actual_token->column);

    fclose(profile);
}
