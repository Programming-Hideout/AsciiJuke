CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
INCLUDES = $(addprefix -I,$(dir $(shell find src -name "*.h")))
LIBS =
LINKS =

SOURCES = $(shell find src -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

VPATH = .:src:src/*

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKS) -c $< -o $@ $(LINKS)

all: $(OBJECTS)
