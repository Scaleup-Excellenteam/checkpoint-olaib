CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS =

SRC = main.c    # Add all your source files here
OBJ = $(SRC:.c=.o)
EXECUTABLE = main
VALGRIND_LOG = valgrind-out.txt

.PHONY: all clean valgrind

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECUTABLE) $(VALGRIND_LOG)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(VALGRIND_LOG) ./$(EXECUTABLE)
