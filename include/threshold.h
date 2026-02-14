#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "lexer.h"

int threshold_check(const char *error_type,
                   const char *expected_type,
                   const char *expected_lexeme,
                   const Token *actual_token);

#endif /* THRESHOLD_H */
