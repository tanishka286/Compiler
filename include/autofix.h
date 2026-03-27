#ifndef AUTOFIX_H
#define AUTOFIX_H

#include "config.h"
#include "lexer.h"

typedef enum {
    AUTOFIX_NOT_APPLIED,
    AUTOFIX_APPLIED
} AutofixResult;

AutofixResult autofix_try(const char *error_type,
                         const char *expected_type,
                         const char *expected_lexeme,
                         const Token *actual_token);

int is_safe_autofix_error(const char *expected_token, const char *actual_token);
void autofix_reset_count(void);
int autofix_limit_reached(void);
void autofix_record_applied(void);
void autofix_reset_lines(void);
int autofix_already_applied_on_line(int line);
void autofix_record_line(int line);

#endif /* AUTOFIX_H */
