// Threshold decision logic

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "threshold.h"

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

static int matches_fingerprint(const char *line,
                               const char *error_type,
                               const char *expected_type,
                               const char *expected_lexeme,
                               const char *actual_type,
                               const char *actual_lexeme) {
    /* Format in file: error_type|expected_type|expected_lexeme|actual_type|actual_lexeme|line|column */
    char expected_fingerprint[512];
    snprintf(expected_fingerprint, sizeof(expected_fingerprint), "%s|%s|%s|%s|%s|",
             error_type ? error_type : "",
             expected_type ? expected_type : "",
             expected_lexeme ? expected_lexeme : "",
             actual_type ? actual_type : "",
             actual_lexeme ? actual_lexeme : "");
    
    /* Check if line starts with the fingerprint (ignoring line/column at the end) */
    return (strncmp(line, expected_fingerprint, strlen(expected_fingerprint)) == 0);
}

int threshold_check(const char *error_type,
                   const char *expected_type,
                   const char *expected_lexeme,
                   const Token *actual_token) {
    FILE *profile = fopen("data/user_profile.dat", "r");
    if (profile == NULL) {
        return 0; /* No history file, not a repeated error */
    }

    const char *actual_type = token_type_to_string(actual_token->type);
    const char *actual_lexeme = actual_token->lexeme;
    
    int count = 0;
    char line[512];
    
    while (fgets(line, sizeof(line), profile) != NULL) {
        /* Remove newline if present */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (matches_fingerprint(line, error_type, expected_type, expected_lexeme,
                                actual_type, actual_lexeme)) {
            count++;
        }
    }
    
    fclose(profile);
    
    /* Return TRUE if count >= 3, FALSE otherwise */
    return (count >= 3);
}
