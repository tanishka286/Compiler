#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include "lexer.h"

void highlight_error(const char *error_type,
                     const char *expected_type,
                     const char *expected_lexeme,
                     const Token *actual_token);

#endif /* HIGHLIGHTER_H */
