EXEC = encode decode
CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -Werror -Wstrict-prototypes -gdwarf-4 -g
OBJECTS = trie.o word.o io.o buffers.o
LDFLAGS = -lm

all: encode decode

encode: encode.o $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

decode: decode.o $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.c *.h

scan-build: clean
	scan-build make


