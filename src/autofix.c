// Temporary auto-correction

#include <string.h>
#include "config.h"
#include "lexer.h"
#include "autofix.h"

static int autofix_count = 0;
static int autofixed_lines[MAX_AUTOFIX_LINES];
static int autofixed_line_count = 0;

void autofix_reset_count(void) {
    autofix_count = 0;
}

int autofix_limit_reached(void) {
    return autofix_count >= MAX_AUTOFIX_PER_RUN;
}

void autofix_record_applied(void) {
    autofix_count++;
}

void autofix_reset_lines(void) {
    autofixed_line_count = 0;
}

int autofix_already_applied_on_line(int line) {
    int i;

    for (i = 0; i < autofixed_line_count; i++) {
        if (autofixed_lines[i] == line) {
            return 1;
        }
    }

    return 0;
}

void autofix_record_line(int line) {
    if (autofixed_line_count >= MAX_AUTOFIX_LINES) {
        return;
    }

    autofixed_lines[autofixed_line_count] = line;
    autofixed_line_count++;
}

int is_safe_autofix_error(const char *expected_token, const char *actual_token) {
    if (expected_token == NULL || actual_token == NULL) {
        return 0;
    }

    if (strcmp(expected_token, ";") != 0) {
        return 0;
    }

    if (strcmp(actual_token, "TOKEN_IDENTIFIER") == 0) {
        return 1;
    }

    if (strcmp(actual_token, "}") == 0) {
        return 1;
    }

    return 0;
}

AutofixResult autofix_try(const char *error_type,
                         const char *expected_type,
                         const char *expected_lexeme,
                         const Token *actual_token) {
    /* Only auto-fix missing semicolon errors */
    if (strcmp(error_type, "syntax_error") != 0) {
        return AUTOFIX_NOT_APPLIED;
    }
    
    /* Check if expected token is semicolon */
    if (expected_type == NULL || strcmp(expected_type, "TOKEN_SYMBOL") != 0) {
        return AUTOFIX_NOT_APPLIED;
    }
    
    if (expected_lexeme == NULL) {
        return AUTOFIX_NOT_APPLIED;
    }
    
    /* Auto-fix case 1 & 2: Missing semicolon at end of statement and before brace */
    if (strcmp(expected_lexeme, ";") == 0) {
        return AUTOFIX_APPLIED;
    }
    
    /* Auto-fix case 3: Missing semicolon at end of statement (when we get EOF) */
    /* Note: We don't auto-fix EOF cases as they're not safe to continue */
    
    return AUTOFIX_NOT_APPLIED;
}
