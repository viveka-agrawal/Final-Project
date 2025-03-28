EXEC = mathlib-test
OBJECTS= mathlib-test.o bbp.o e.o euler.o madhava.o newton.o viete.o

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) bbp.o e.o euler.o madhava.o newton.o viete.o mathlib-test.o -o mathlib-test $(LDFLAGS) 

bbp.o: bbp.c mathlib.h
	$(CC) $(CFLAGS) -c bbp.c

e.o: e.c mathlib.h
	$(CC) $(CFLAGS) -c e.c

euler.o: euler.c mathlib.h
	$(CC) $(CFLAGS) -c euler.c

madhava.o: madhava.c mathlib.h
	$(CC) $(CFLAGS) -c madhava.c

newton.o: newton.c mathlib.h
	$(CC) $(CFLAGS) -c newton.c

viete.o: viete.c mathlib.h
	$(CC) $(CFLAGS) -c viete.c

mathlib-test.o: mathlib-test.c bbp.c e.c euler.c madhava.c newton.c viete.c mathlib.h
	$(CC) $(CFLAGS) -c mathlib-test.c


clean:
	rm -f mathlib-test $(OBJECTS)

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]
