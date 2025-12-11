CFILES = $(shell find src/ -name "*.c")
OFILES = $(CFILES:.c=.o)
CFLAGS = -Wall -pedantic -Iinclude/
CC = clang

.PHONY: all
all: $(OFILES)
	$(CC) $(OFILES) -lm -o med

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
