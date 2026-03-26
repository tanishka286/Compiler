# Makefile
CC = gcc
CFLAGS = -Iinclude

SRC = src/main.c src/lexer.c src/parser.c src/error_tracker.c src/threshold.c src/autofix.c src/highlighter.c
OUT = build/hasc.exe

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	del build\hasc.exe