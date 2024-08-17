# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SOURCES = example.c helper.c

# Output binary
OUTPUT = myprogram

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	rm -f $(OUTPUT)
