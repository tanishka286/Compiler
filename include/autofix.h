#ifndef AUTOFIX_H
#define AUTOFIX_H

#include "lexer.h"

typedef enum {
    AUTOFIX_NOT_APPLIED,
    AUTOFIX_APPLIED
} AutofixResult;

AutofixResult autofix_try(const char *error_type,
                         const char *expected_type,
                         const char *expected_lexeme,
                         const Token *actual_token);

#endif /* AUTOFIX_H */
