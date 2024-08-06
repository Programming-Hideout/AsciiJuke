CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
INCLUDES = $(addprefix -I,$(dir $(shell find src -name "*.h")))
LIBS =
LINKS =
APP = juke

SOURCES = $(shell find src -name "*.c")
OBJECTS = $(SOURCES:.c=.o)

VPATH = .:src:src/*

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKS) -c $< -o $@ $(LINKS)

$(APP): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKS) $^ -o $@ $(LINKS)
