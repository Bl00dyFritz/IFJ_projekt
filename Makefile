EXEC = compiler

CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra
LDLIBS= -lm

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
DEPS = $(SRCS:.c=.d)

.PHONY: clean all

$(EXEC):compiler.c ast.c parser.c generator.c scanner.c semantics.c stack.c symtable.c

-include $(DEPS)

deps:*.c
	gcc -MM *.c > dep

clean:
	rm -f $(EXEC) $(DEPS) $(OBJS)
