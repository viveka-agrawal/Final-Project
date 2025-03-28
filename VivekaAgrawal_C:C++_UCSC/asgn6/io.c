#include "io.h"
#include "code.h"
#include "buffers.h"
#include "endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>

uint64_t total_syms; // To count the symbols processed
uint64_t total_bits; // To count the bits processed

// helper function to perform reads
int read_bytes(int infile, uint8_t *buf, int to_read) {
    ssize_t n = 0;
    int totalread = 0;

    // while total number of bytes read is less than the number of bytes to read and end of file is not reached
    while (totalread < to_read) {
        // set number of bytes read to n
        n = read(infile, buf, to_read);

        // if number of bytes read is 0, end of file is reached
        if (n == 0) {
            return totalread;
        }

        // or if there is an error in reading the file (-1)
        else if (n == -1) {
            printf("file read error %s\n", strerror(errno));
            return 0;
        }

        else {
            // else increment the total number of bytes read
            totalread += n;
        }
    }

    return totalread;
}

// helper function to perform writes
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    ssize_t n = 0;
    int totalwrite = 0;

    // while total number of bytes written is less than the number of bytes to write and end of file is not reached
    while (totalwrite < to_write) {
        // set number of bytes written to n
        n = write(outfile, buf, to_write);

        // if number of bytes written is -1, there is an error in writing to file
        if (n == -1) {
            return 0;
        } else {
            // else increment the total number of bytes written
            totalwrite += n;
        }
    }

    return totalwrite;
}

// function to read in FileHeader bytes
void read_header(int infile, FileHeader *header) {
    uint16_t fh_size = 0;

    // calculate size of file header
    fh_size = sizeof(FileHeader);

    // read the fh_size of bytes into the header
    read_bytes(infile, (uint8_t *) header, fh_size);

    // check for endianness
    if (big_endian() == true) {
        // swap the bytes on a big endian machine
        swap32(header->magic);
        swap16(header->protection);
    }

    // check the magic number
    if (header->magic != MAGIC) {
        printf("magic number does not match MAGIC.\n");
    }
}

// function to write FileHeader bytes
void write_header(int outfile, FileHeader *header) {
    uint16_t fh_size = 0;

    // calculate size of file header
    fh_size = sizeof(FileHeader);

    // check for endianness
    if (big_endian() == true) {
        // swap the bytes on a big endian machine
        swap32(header->magic);
        swap16(header->protection);
    }

    // write the fh_size of bytes into the outfile
    write_bytes(outfile, (uint8_t *) header, fh_size);
}

// function to read symbols from infile
bool read_sym(int infile, uint8_t *sym) {
    int numbytes = 0;

    // check if a new 4K BLOCK has to be read
    if (chararrindex == BLOCK || total_syms == 0) {
        // read 4K BLOCK from input file
        numbytes = read_bytes(infile, chararr, BLOCK);

        // reset character array index to 0
        chararrindex = 0;
        total_syms += numbytes;

        if (numbytes == 0) {
            // no more symbols to read
            return false;
        }

        // check for infile data less than 4K BLOCK
        if (numbytes < BLOCK) {
            // mark the end of the symbol buffer
            chararr[numbytes] = '\0';
        }

    } // end of outermost if

    // check if valid character is present in symbol buffer
    if (chararr[chararrindex] != '\0' || chararr[chararrindex] != 0) {
        // store current symbol from character array in sym
        *sym = chararr[chararrindex];
        chararrindex++;

        // more symbols to read
        return true;
    }

    else {
        // no more symbols to read
        return false;
    }
}

// function to write pair to outfile
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    // store the bits of code in buf and write out bytes to binpair buffer
    for (uint8_t i = 0; i < bitlen; i++) {
        // check if LSB in code is 1
        if (code & 0x0001) {
            // set bit to 1 at the location of 2^bitcount in buf
            buf = buf | (1 << bitcount);
        }

        // look at next bit in the code
        code = code >> 1;
        bitcount++;

        // check if 1 byte has been created
        // store the byte in binpair array
        if (bitcount == 8) {
            binpair[binpairindex] = buf;
            binpairindex++;
            total_bits += 8;

            // check if binpair array is full
            // write out 4K BLOCK to outfile
            if (binpairindex == BLOCK) {
                flush_pairs(outfile);
            }

            // clear out the temporary buffer buf
            buf = buf & 0x00;

            // reset the bitcount
            bitcount = 0;
        }
    } // end of for loop for code

    // store the bits of sym in buf and write out bytes to binpair buff
    for (uint8_t i = 0; i < 8; i++) {
        // check if LSB in sym is 1
        if (sym & 0x01) {
            // set bit to 1 at the location of 2^bitcount in buf
            buf = buf | (1 << bitcount);
        }

        // look at next bit in the symbol
        sym = sym >> 1;
        bitcount++;

        // check if 1 byte has been created
        // store the byte in binpair array
        if (bitcount == 8) {
            binpair[binpairindex] = buf;
            binpairindex++;
            total_bits += 8;

            // check if binpair array is full
            // write out 4K BLOCK to outfile
            if (binpairindex == BLOCK) {
                flush_pairs(outfile);
            }

            // clear out the temporary buffer buf
            buf = buf & 0x00;

            // reset the bitcount
            bitcount = 0;
        }
    }
}

// function to write out remaining pairs of symbols and codes
void flush_pairs(int outfile) {
    uint8_t numpadbits = 8 - bitcount;

    // check if padding bits are required to complete the byte
    if (numpadbits > 0) {

        // check if there are bits in buffer buf
        // complete the byte with 0 padding
        for (uint16_t i = 0; i < numpadbits; i++) {
            // clearing the bit located at 2^bitcount
            buf = buf & ~(1 << bitcount);
            bitcount++;
        }

        // write out the byte to binpair array
        binpair[binpairindex] = buf;

        binpairindex++;
    }

    // write out the remaining bytes from binpair array to outfile
    write_bytes(outfile, binpair, binpairindex);

    // reset binpairindex
    binpairindex = 0;

    // clear out the temporary buffer buf
    buf = buf & 0x00;

    // reset bitcount
    bitcount = 0;
}

// read pair from input file
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    // initialize values to 0
    int numbytes = 0;
    *code = 0;
    *sym = 0;

    // check if a new 4K BLOCK has to be read
    if (binpairindex == BLOCK || total_bits == 0) {

        // read 4K BLOCK from input file
        numbytes = read_bytes(infile, binpair, BLOCK);

        // reset binpair index
        binpairindex = 0;

        total_bits += (numbytes * 8);

        if (numbytes == 0) {
            // no more pairs to read
            return false;
        }

        // store current byte from binpair array in buf byte buffer
        buf = binpair[binpairindex];

        binpairindex++;
    }

    // read the bits of code from binpair array
    for (uint8_t i = 0; i < bitlen; i++) {
        // check if LSB in buf is 1
        if (buf & 0x01) {
            // add current bitvalue 2^i to code value
            *code = *code + (1 << i);
        }

        bitcount++;

        // check if end of byte has not been reached
        if (bitcount < 8) {
            buf = buf >> 1;
        }

        // check if 1 byte has been read
        // read the next byte from the binpair array
        if (bitcount == 8) {
            // clear out the temporary buffer buf
            buf = buf & 0x00;

            buf = binpair[binpairindex];
            binpairindex++;
            total_bits += 8;

            // reset the bitcount
            bitcount = 0;
        }
    } // end of for loop for code

    // read the bits of sym from binpair array
    for (uint8_t i = 0; i < 8; i++) {
        // check if LSB in buf is 1
        if (buf & 0x01) {
            // add current bitvalue 2^i to sym value
            *sym = *sym + (1 << i);
        }

        bitcount++;

        // check if end of byte has not been reached
        if (bitcount < 8) {
            buf = buf >> 1;
        }

        // check if 1 byte has been read
        // read the next byte from the binpair array
        if (bitcount == 8) {
            // clear out the temporary buffer buf
            buf = buf & 0x00;

            buf = binpair[binpairindex];

            binpairindex++;
            total_bits += 8;

            // reset the bitcount
            bitcount = 0;
        }

    } // end of sym for loop

    // check if there are pairs left to read
    if (*code == STOP_CODE) {
        // no more pairs left to read
        return false;
    } else {
        // more pairs left to read
        return true;
    }
}

// function to write Word to outfile
void write_word(int outfile, Word *w) {
    // for loop to write symbols from words into character array buffer
    for (uint16_t i = 0; i < w->len; i++) {
        chararr[chararrindex] = w->syms[i];
        chararrindex++;
        total_syms++;

        // check if chararr array is full
        // write out 4K BLOCK to outfile
        if (chararrindex == BLOCK) {
            flush_words(outfile);
        }
    }
}

// function to write out any remaining symbols in character array buffer to outfile
void flush_words(int outfile) {
    // write out the remaining bytes from chararr array to outfile
    write_bytes(outfile, chararr, chararrindex);

    // reset chararrindex
    chararrindex = 0;
}
