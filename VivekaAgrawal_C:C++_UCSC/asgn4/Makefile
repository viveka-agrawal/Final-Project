EXEC = life
OBJECTS= universe.o life.o

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes -gdwarf-4
LDFLAGS = -lncurses

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) universe.o life.o -o life $(LDFLAGS) 

universe.o: universe.h universe.c
	$(CC) $(CFLAGS) -c universe.c

life.o: life.h life.c universe.c
	$(CC) $(CFLAGS) -c life.c

clean:
	rm -f life $(OBJECTS)

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

