#ifndef ERROR_TRACKER_H
#define ERROR_TRACKER_H

#include "lexer.h"

void error_tracker_log(const char *error_type,
                       const char *expected_type,
                       const char *expected_lexeme,
                       const Token *actual_token);

#endif /* ERROR_TRACKER_H */
