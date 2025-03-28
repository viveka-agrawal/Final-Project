#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "trie.h"
#include "code.h"
#include "endian.h"
#include "io.h"
#include "buffers.h"
#define OPTIONS "hi:o:v"

void help(void) {
    printf("SYNOPSIS\n");
    printf("   Decompresses files with the LZ78 decompression algorithm.\n");
    printf("   Used with files compressed with the corresponding encoder.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./decode [-vh] [-i input] [-o output]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -v              Display decompression statistics.\n");
    printf("   -i input        Specify input to decompress (stdin by default).\n");
    printf("   -o output       Specify output of decompressed input (stdout by default).\n");
    printf("   -h              Display program help and usage.\n");
}

// Pseudocode provided by Professor Long

int main(int argc, char **argv) {
    int infile = 0;
    int outfile = 1;
    char outfilename[512];
    strcpy(outfilename, "");

    FileHeader header;
    struct stat statbuf;

    bool verbose = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg != NULL) { // check for NULL optarg
                infile = open(
                    optarg, O_RDONLY); // open infile to decode based on command line argument
                if (infile == -1) {
                    printf("Error: failed to open input compressed file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'o':
            if (optarg != NULL) { // check for NULL optarg
                strcpy(outfilename, optarg);
            }
            break;
        case 'v': verbose = true; break;
        case 'h':
            help();
            return 0;
            break;
        case '?': // if optarg argument is not present for -i or -o options in command line
            if (optopt == 'i' || optopt == 'o') {
                help();
                return 0;
            }
            break;
        default: break;
        } // end of switch statement
    } // end of while loop

    // read the file header from infile
    read_header(infile, &header);

    // verify the magic value in the infile header
    if (header.magic != MAGIC) {
        printf("MAGIC header not verified in infile.\n");
        return -1;
    }

    if (strlen(outfilename) > 1) {
        outfile = open(outfilename,
            O_WRONLY | O_CREAT); // open outfile to decode based on command line argument
    } else {
        outfile = 1; // use outfile with standard output
    }

    if (outfile == -1) {
        printf("Error: failed to open output decompressed file. %s\n", optarg);
        return -1;
    }

    // initialize buffers
    init_buffers();

    // read the stats bits from infile
    fstat(infile, &statbuf);

    // store the file size of compressed infile
    off_t infilesize = statbuf.st_size;

    // set permissions for outfile (same as infile)
    fchmod(outfile, header.protection);

    WordTable *table;
    uint16_t curr_code, next_code, bitlength;
    uint8_t curr_sym;

    // create WordTable
    table = wt_create();

    next_code = START_CODE;

    bitlength = log2(next_code) + 1; // calculating bit-length of next_code

    // run while loop to process pairs
    while (read_pair(infile, &curr_code, &curr_sym, bitlength) == true) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);

        write_word(outfile, table[next_code]);

        next_code++;

        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }

        bitlength = log2(next_code) + 1; // calculating bit-length of next_code

    } // end of while loop

    flush_words(outfile);

    // print verbose output if -v option is specified
    if (verbose == true) {

        // read the stats bits from outfile
        fstat(outfile, &statbuf);

        // store the file size of uncompressed outfile
        off_t outfilesize = statbuf.st_size;

        // calculate space saving
        double ss;
        double in, out;

        in = infilesize;
        out = outfilesize;

        ss = 100 * (1 - (in / out));

        printf("Compressed file size: %lu bytes\n", (unsigned long int) in);
        printf("Uncompressed file size: %lu bytes\n", (unsigned long int) out);
        printf("Space saving: %2.2lf %% \n", ss);
    }

    close(infile);
    close(outfile);
    wt_delete(table);
    return 0;
}
