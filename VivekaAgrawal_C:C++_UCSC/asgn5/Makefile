EXEC = keygen encrypt decrypt
CC= clang
CFLAGS= -Wall -Wpedantic -Wextra -Werror -Wstrict-prototypes -gdwarf-4 -g $(shell pkg-config --cflags gmp)
LDFLAGS = $(shell pkg-config --libs gmp)
OBJECTS= numtheory.o randstate.o ss.o

all: keygen encrypt decrypt

keygen: keygen.o randstate.o numtheory.o ss.o $(OBEJCTS)
	$(CC) -o $@ $^ $(LDFLAGS)

encrypt: encrypt.o $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

decrypt: decrypt.o $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt *.o

cleankeys:
	rm -f *. {pub,priv}

format:
	clang-format -i -style=file *.c *.h

scan-build: clean
	scan-build make

