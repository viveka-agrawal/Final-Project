EXEC = sorting
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -O2
LDFLAGS = -lm

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c $<

heap.o: heap.c
	$(CC) $(CFLAGS) -c $<

quick.o: quick.c
	$(CC) $(CFLAGS) -c $<

shell.o: shell.c
	$(CC) $(CFLAGS) -c $<

tidy:
	rm -rf $(OBJECTS)

clean: tidy
	rm -rf $(EXEC)

format:
	clang-format -i -style=file *.c *.h

