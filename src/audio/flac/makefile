SOURCES = $(wildcard *.c) ../../utils/endians.c

%.o: %.c
	gcc -Wall -Wextra -I ../../utils/ -g -c $< -o $@

app: $(SOURCES:.c=.o)
	gcc -Wall -Wextra -I ../../utils/ -g $^ -o $@
