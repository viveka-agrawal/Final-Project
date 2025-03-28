#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include "code.h"
#include "io.h"

static uint32_t binpairindex; // index for binary pair array
static uint32_t chararrindex; // index for character array
static uint8_t buf; // temporary 1 byte buffer to process bits
static uint8_t bitcount; // index for bits stored in buf

static uint8_t binpair[BLOCK]; // buffer to store binary pairs
static uint8_t chararr[BLOCK]; // buffer to store characters

// function to initialize buffers binpair array and character array
void init_buffers(void);
