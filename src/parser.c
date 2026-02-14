// Syntax analyzer

#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "error_tracker.h"
#include "threshold.h"
#include "autofix.h"
#include "highlighter.h"

static const char* parser_token_type_to_string(TokenType type) {
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

void parser_init(void) {
    /* No parser state to initialize yet */
}

static AutofixResult report_syntax_error(const char *expected_type,
                                         const char *expected_lexeme,
                                         const Token *token) {
    printf("Syntax error: expected %s \"%s\" but got %s \"%s\" "
           "at line %d, column %d\n",
           expected_type,
           expected_lexeme,
           parser_token_type_to_string(token->type),
           token->lexeme,
           token->line,
           token->column);
    
    error_tracker_log("syntax_error", expected_type, expected_lexeme, token);
    
    if (threshold_check("syntax_error", expected_type, expected_lexeme, token)) {
        printf("Notice: This appears to be a repeated (habitual) mistake.\n");
        if (autofix_try("syntax_error", expected_type, expected_lexeme, token) == AUTOFIX_APPLIED) {
            printf("Auto-fix applied (execution-only): missing ';'\n");
            highlight_error("syntax_error", expected_type, expected_lexeme, token);
            return AUTOFIX_APPLIED;
        }
    }
    
    highlight_error("syntax_error", expected_type, expected_lexeme, token);
    return AUTOFIX_NOT_APPLIED;
}

void parse_program(void) {
    Token token;

    /* Expect: int */
    token = get_next_token();
    if (!(token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "int") == 0)) {
        report_syntax_error("TOKEN_KEYWORD", "int", &token);
        return;
    }

    /* Expect: main */
    token = get_next_token();
    if (!(token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "main") == 0)) {
        report_syntax_error("TOKEN_KEYWORD", "main", &token);
        return;
    }

    /* Expect: ( */
    token = get_next_token();
    if (!(token.type == TOKEN_SYMBOL && strcmp(token.lexeme, "(") == 0)) {
        report_syntax_error("TOKEN_SYMBOL", "(", &token);
        return;
    }

    /* Expect: ) */
    token = get_next_token();
    if (!(token.type == TOKEN_SYMBOL && strcmp(token.lexeme, ")") == 0)) {
        report_syntax_error("TOKEN_SYMBOL", ")", &token);
        return;
    }

    /* Expect: { */
    token = get_next_token();
    if (!(token.type == TOKEN_SYMBOL && strcmp(token.lexeme, "{") == 0)) {
        report_syntax_error("TOKEN_SYMBOL", "{", &token);
        return;
    }

    /* Parse stmt_list (possibly empty) and the closing '}' */
    for (;;) {
        /* Look at the next token to decide: '}' ends block, otherwise a stmt */
        token = get_next_token();

        if (token.type == TOKEN_EOF) {
            printf("Syntax error: unexpected EOF inside block at line %d, column %d "
                   "(expected '}' or ';')\n",
                   token.line,
                   token.column);
            error_tracker_log("syntax_error", "TOKEN_SYMBOL", "} or ;", &token);
            if (threshold_check("syntax_error", "TOKEN_SYMBOL", "} or ;", &token)) {
                printf("Notice: This appears to be a repeated (habitual) mistake.\n");
            }
            highlight_error("syntax_error", "TOKEN_SYMBOL", "} or ;", &token);
            return;
        }

        /* End of block: no more statements */
        if (token.type == TOKEN_SYMBOL && strcmp(token.lexeme, "}") == 0) {
            break;
        }

        /* Start of a statement */
        if (token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "int") == 0) {
            /* Declaration statement: int <identifier> ; */
            Token ident = get_next_token();
            if (ident.type != TOKEN_IDENTIFIER) {
                report_syntax_error("TOKEN_IDENTIFIER", "<identifier>", &ident);
                return;
            }

            Token semi = get_next_token();
            if (!(semi.type == TOKEN_SYMBOL && strcmp(semi.lexeme, ";") == 0)) {
                if (report_syntax_error("TOKEN_SYMBOL", ";", &semi) == AUTOFIX_APPLIED) {
                    /* Simulate semicolon consumed, continue parsing */
                    if (semi.type == TOKEN_SYMBOL && strcmp(semi.lexeme, "}") == 0) {
                        /* '}' will be handled by outer loop - set token and break */
                        token = semi;
                        break;
                    }
                }
                return;
            }

            /* Valid declaration statement consumed; continue with next statement or '}' */
            continue;
        }

        if (token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "if") == 0) {
            /* If statement: if ( IDENTIFIER | NUMBER ) { } */
            Token lparen_if = get_next_token();
            if (!(lparen_if.type == TOKEN_SYMBOL && strcmp(lparen_if.lexeme, "(") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "(", &lparen_if);
                return;
            }

            Token cond = get_next_token();
            if (!(cond.type == TOKEN_IDENTIFIER || cond.type == TOKEN_NUMBER)) {
                report_syntax_error("TOKEN_IDENTIFIER or TOKEN_NUMBER",
                                    "<identifier or number>",
                                    &cond);
                return;
            }

            Token rparen_if = get_next_token();
            if (!(rparen_if.type == TOKEN_SYMBOL && strcmp(rparen_if.lexeme, ")") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", ")", &rparen_if);
                return;
            }

            Token lbrace_if = get_next_token();
            if (!(lbrace_if.type == TOKEN_SYMBOL && strcmp(lbrace_if.lexeme, "{") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "{", &lbrace_if);
                return;
            }

            Token rbrace_if = get_next_token();
            if (!(rbrace_if.type == TOKEN_SYMBOL && strcmp(rbrace_if.lexeme, "}") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "}", &rbrace_if);
                return;
            }

            /* Valid if statement consumed; continue with next statement or '}' */
            continue;
        }

        if (token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "while") == 0) {
            /* While statement: while ( IDENTIFIER | NUMBER ) { } */
            Token lparen_while = get_next_token();
            if (!(lparen_while.type == TOKEN_SYMBOL && strcmp(lparen_while.lexeme, "(") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "(", &lparen_while);
                return;
            }

            Token cond_while = get_next_token();
            if (!(cond_while.type == TOKEN_IDENTIFIER || cond_while.type == TOKEN_NUMBER)) {
                report_syntax_error("TOKEN_IDENTIFIER or TOKEN_NUMBER",
                                    "<identifier or number>",
                                    &cond_while);
                return;
            }

            Token rparen_while = get_next_token();
            if (!(rparen_while.type == TOKEN_SYMBOL && strcmp(rparen_while.lexeme, ")") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", ")", &rparen_while);
                return;
            }

            Token lbrace_while = get_next_token();
            if (!(lbrace_while.type == TOKEN_SYMBOL && strcmp(lbrace_while.lexeme, "{") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "{", &lbrace_while);
                return;
            }

            Token rbrace_while = get_next_token();
            if (!(rbrace_while.type == TOKEN_SYMBOL && strcmp(rbrace_while.lexeme, "}") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "}", &rbrace_while);
                return;
            }

            /* Valid while statement consumed; continue with next statement or '}' */
            continue;
        }

        if (token.type == TOKEN_KEYWORD && strcmp(token.lexeme, "print") == 0) {
            /* Print statement: print ( IDENTIFIER | NUMBER ) ; */
            Token lparen = get_next_token();
            if (!(lparen.type == TOKEN_SYMBOL && strcmp(lparen.lexeme, "(") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "(", &lparen);
                return;
            }

            Token value = get_next_token();
            if (!(value.type == TOKEN_IDENTIFIER || value.type == TOKEN_NUMBER)) {
                report_syntax_error("TOKEN_IDENTIFIER or TOKEN_NUMBER",
                                    "<identifier or number>",
                                    &value);
                return;
            }

            Token rparen = get_next_token();
            if (!(rparen.type == TOKEN_SYMBOL && strcmp(rparen.lexeme, ")") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", ")", &rparen);
                return;
            }

            Token semi_print = get_next_token();
            if (!(semi_print.type == TOKEN_SYMBOL && strcmp(semi_print.lexeme, ";") == 0)) {
                if (report_syntax_error("TOKEN_SYMBOL", ";", &semi_print) == AUTOFIX_APPLIED) {
                    /* Simulate semicolon consumed, continue parsing */
                    if (semi_print.type == TOKEN_SYMBOL && strcmp(semi_print.lexeme, "}") == 0) {
                        /* '}' will be handled by outer loop - set token and break */
                        token = semi_print;
                        break;
                    }
                }
                return;
            }

            /* Valid print statement consumed; continue with next statement or '}' */
            continue;
        }

        if (token.type == TOKEN_IDENTIFIER) {
            /* Assignment statement: <identifier> = <number> ; */
            Token eq = get_next_token();
            if (!(eq.type == TOKEN_SYMBOL && strcmp(eq.lexeme, "=") == 0)) {
                report_syntax_error("TOKEN_SYMBOL", "=", &eq);
                return;
            }

            Token number = get_next_token();
            if (number.type != TOKEN_NUMBER) {
                report_syntax_error("TOKEN_NUMBER", "<number>", &number);
                return;
            }

            Token semi2 = get_next_token();
            if (!(semi2.type == TOKEN_SYMBOL && strcmp(semi2.lexeme, ";") == 0)) {
                if (report_syntax_error("TOKEN_SYMBOL", ";", &semi2) == AUTOFIX_APPLIED) {
                    /* Simulate semicolon consumed, continue parsing */
                    if (semi2.type == TOKEN_SYMBOL && strcmp(semi2.lexeme, "}") == 0) {
                        /* '}' will be handled by outer loop - set token and break */
                        token = semi2;
                        break;
                    }
                }
                return;
            }

            /* Valid assignment statement consumed; continue with next statement or '}' */
            continue;
        }

        /* Non-declaration, non-assignment statement: consume tokens until we find ';' */
        while (!(token.type == TOKEN_SYMBOL && strcmp(token.lexeme, ";") == 0)) {
            token = get_next_token();

            if (token.type == TOKEN_EOF) {
                printf("Syntax error: unexpected EOF in statement at line %d, column %d "
                       "(expected ';' or '}')\n",
                       token.line,
                       token.column);
                error_tracker_log("syntax_error", "TOKEN_SYMBOL", "; or }", &token);
                if (threshold_check("syntax_error", "TOKEN_SYMBOL", "; or }", &token)) {
                    printf("Notice: This appears to be a repeated (habitual) mistake.\n");
                }
                highlight_error("syntax_error", "TOKEN_SYMBOL", "; or }", &token);
                return;
            }

            if (token.type == TOKEN_SYMBOL && strcmp(token.lexeme, "}") == 0) {
                printf("Syntax error: unexpected '}' in statement at line %d, column %d "
                       "(missing ';' before closing brace)\n",
                       token.line,
                       token.column);
                error_tracker_log("syntax_error", "TOKEN_SYMBOL", ";", &token);
                if (threshold_check("syntax_error", "TOKEN_SYMBOL", ";", &token)) {
                    printf("Notice: This appears to be a repeated (habitual) mistake.\n");
                    if (autofix_try("syntax_error", "TOKEN_SYMBOL", ";", &token) == AUTOFIX_APPLIED) {
                        printf("Auto-fix applied (execution-only): missing ';'\n");
                        highlight_error("syntax_error", "TOKEN_SYMBOL", ";", &token);
                        /* Simulate semicolon consumed, then handle '}' in outer loop */
                        break; /* Exit statement loop, '}' will be handled by outer loop */
                    }
                }
                highlight_error("syntax_error", "TOKEN_SYMBOL", ";", &token);
                return;
            }
        }
        /* ';' consumed: one non-declaration statement completed; continue to look for more or '}' */
    }

    printf("Program with statements parsed successfully\n");
}

void parser_close(void) {
    /* No parser resources to release yet */
}

