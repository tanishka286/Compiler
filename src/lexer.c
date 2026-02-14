#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static FILE *source_file = NULL;
static int current_line = 1;
static int current_column = 0;

static int is_keyword(const char *lexeme) {
    return (strcmp(lexeme, "int") == 0 ||
            strcmp(lexeme, "if") == 0 ||
            strcmp(lexeme, "else") == 0 ||
            strcmp(lexeme, "while") == 0 ||
            strcmp(lexeme, "print") == 0 ||
            strcmp(lexeme, "main") == 0);
}

static int is_single_symbol(int c) {
    return (c == ';' || c == '{' || c == '}' || c == '(' || c == ')' ||
            c == '=' || c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '<' || c == '>');
}

void init_lexer(const char *filename) {
    source_file = fopen(filename, "r");
    if (source_file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        exit(1);
    }
    current_line = 1;
    current_column = 0;
}

Token get_next_token(void) {
    Token token;
    int c;
    
    // Skip whitespace
    while ((c = fgetc(source_file)) != EOF) {
        current_column++;
        
        if (c == '\n') {
            current_line++;
            current_column = 0;
        } else if (c == ' ' || c == '\t') {
            // Continue skipping whitespace
        } else {
            // Non-whitespace character found
            if (c != EOF && isalpha((unsigned char)c)) {
                // Read identifier or keyword
                int start_line = current_line;
                int start_column = current_column - 1; // Position of first character
                int len = 0;
                
                // Read the first character (already consumed)
                token.lexeme[len++] = (char)c;
                
                // Read remaining alphanumeric characters
                while ((c = fgetc(source_file)) != EOF && isalnum((unsigned char)c)) {
                    current_column++;
                    if (len < 63) { // Leave room for null terminator
                        token.lexeme[len++] = (char)c;
                    }
                }
                
                // Push back the non-alphanumeric character
                if (c != EOF) {
                    ungetc(c, source_file);
                    current_column--;
                }
                
                token.lexeme[len] = '\0';
                token.line = start_line;
                token.column = start_column;
                
                // Classify as keyword or identifier
                if (is_keyword(token.lexeme)) {
                    token.type = TOKEN_KEYWORD;
                } else {
                    token.type = TOKEN_IDENTIFIER;
                }
                
                return token;
            } else if (c != EOF && isdigit((unsigned char)c)) {
                // Read numeric literal
                int start_line = current_line;
                int start_column = current_column - 1; // Position of first digit
                int len = 0;
                
                // Read the first digit (already consumed)
                token.lexeme[len++] = (char)c;
                
                // Read remaining digits
                while ((c = fgetc(source_file)) != EOF && isdigit((unsigned char)c)) {
                    current_column++;
                    if (len < 63) { // Leave room for null terminator
                        token.lexeme[len++] = (char)c;
                    }
                }
                
                // Push back the non-digit character
                if (c != EOF) {
                    ungetc(c, source_file);
                    current_column--;
                }
                
                token.lexeme[len] = '\0';
                token.line = start_line;
                token.column = start_column;
                token.type = TOKEN_NUMBER;
                
                return token;
            } else {
                // Symbols/operators (and fallback TOKEN_UNKNOWN)
                int start_line = current_line;
                int start_column = current_column - 1; // Position of first character

                // Multi-character operators first: == != <= >=
                if (c == '=' || c == '!' || c == '<' || c == '>') {
                    int next = fgetc(source_file);
                    if (next != EOF) {
                        current_column++;
                    }

                    if ((c == '=' && next == '=') ||
                        (c == '!' && next == '=') ||
                        (c == '<' && next == '=') ||
                        (c == '>' && next == '=')) {
                        token.type = TOKEN_SYMBOL;
                        token.lexeme[0] = (char)c;
                        token.lexeme[1] = (char)next;
                        token.lexeme[2] = '\0';
                        token.line = start_line;
                        token.column = start_column;
                        return token;
                    }

                    if (next != EOF) {
                        ungetc(next, source_file);
                        current_column--;
                    }
                }

                // Single-character symbols/operators
                if (is_single_symbol(c)) {
                    token.type = TOKEN_SYMBOL;
                    token.lexeme[0] = (char)c;
                    token.lexeme[1] = '\0';
                    token.line = start_line;
                    token.column = start_column;
                    return token;
                }

                // Truly unsupported character
                token.type = TOKEN_UNKNOWN;
                token.lexeme[0] = (char)c;
                token.lexeme[1] = '\0';
                token.line = start_line;
                token.column = start_column;
                return token;
            }
        }
    }
    
    // EOF reached after skipping whitespace
    token.type = TOKEN_EOF;
    token.lexeme[0] = '\0';
    token.line = current_line;
    token.column = current_column;
    return token;
}

void close_lexer(void) {
    if (source_file != NULL) {
        fclose(source_file);
        source_file = NULL;
    }
}
