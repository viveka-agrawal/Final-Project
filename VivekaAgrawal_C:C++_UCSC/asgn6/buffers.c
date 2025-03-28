#include "buffers.h"
#include "io.h"
#include <stdio.h>

// function to initialize buffers binpair array and character array
void init_buffers(void) {
    // initialize binpair array with 0 values
    for (uint16_t i = 0; i < BLOCK; i++) {
        binpair[i] = 0;
    }

    binpairindex = 0;

    // initialize character array with 0 values
    for (uint16_t i = 0; i < BLOCK; i++) {
        chararr[i] = 0;
    }

    chararrindex = 0;

    // initialize temporary buffer buf and bitcount
    buf = 0x00; // temporary 1 byte buffer to store pair bits
    bitcount = 0; // index for bits stored in buf

    total_syms = 0;
    total_bits = 0;
}
