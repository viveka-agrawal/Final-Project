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
#include <errno.h>
#include <string.h>
#define OPTIONS "hi:o:v"

// Pseudocode provided by Professor Long

void help(void) {
    printf("SYNOPSIS\n");
    printf("   Compresses files using the LZ78 compression algorithm.\n");
    printf("   Compressed files are decompressed with the corresponding decoder.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./encode [-vh] [-i input] [-o output]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -v              Display compression statistics.\n");
    printf("   -i input        Specify input to compress (stdin by default).\n");
    printf("   -o output       Specify output of compressed input (stdout by default).\n");
    printf("   -h              Display program help and usage.\n");
}

int main(int argc, char **argv) {
    int infile = 0;
    int outfile = 1;
    char outfilename[512];
    strcpy(outfilename, "");

    FileHeader *header;
    struct stat statbuf;

    bool verbose = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg != NULL) { // check for NULL optarg
                infile = open(
                    optarg, O_RDONLY); // open infile to encode based on command line argument
                if (infile == -1) {
                    printf("Error: failed to open input compress file. %s\n", optarg);
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

    if (strlen(outfilename) > 1) {
        outfile = open(outfilename,
            O_WRONLY | O_CREAT); // open outfile to encode based on command line argument
    } else {
        outfile = 1; // use outfile with standard output
    }

    if (outfile == -1) {
        printf("Error: failed to open output compressed file. %s\n", optarg);
        return -1;
    }

    // initialize buffers
    init_buffers();

    // read the stats bits from infile
    fstat(infile, &statbuf);

    // store the protection mode of infile in mode
    mode_t mode = statbuf.st_mode;

    // store the file size of uncompressed infile
    off_t infilesize = statbuf.st_size;

    // set permissions for outfile (same as infile)
    fchmod(outfile, mode);

    // dynamically allocate memory for file header
    header = (FileHeader *) calloc(1, sizeof(FileHeader));

    // construct the output file header
    header->magic = MAGIC;
    header->protection = mode;

    // write the header to output file
    write_header(outfile, header);

    TrieNode *rootnode, *curr_node, *prev_node, *next_node;
    uint16_t next_code = START_CODE;
    uint8_t prev_sym, curr_sym;
    int bitlength;

    // Create a Trie
    rootnode = trie_create();

    // Create a copy of rootnode
    curr_node = rootnode;

    // run while loop to process symbols
    while (read_sym(infile, &curr_sym) != false) {
        next_node = trie_step(curr_node, curr_sym);

        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            bitlength = log2(next_code) + 1; // calculating bit-length of next_code
            write_pair(outfile, curr_node->code, curr_sym, bitlength);
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = rootnode;
            next_code++;
        }

        // check if next_code = MAX_CODE
        if (next_code == MAX_CODE) {
            trie_reset(rootnode);
        }

        prev_sym = curr_sym;
    } // end of while loop

    if (curr_node != rootnode) {
        bitlength = log2(next_code) + 1; // calculating bit-length of next_code
        write_pair(outfile, prev_node->code, prev_sym, bitlength);
        next_code++;
        next_code %= MAX_CODE;
    }

    bitlength = log2(next_code) + 1; // calculating bit-length of next_code
    write_pair(outfile, STOP_CODE, 0, bitlength);

    flush_pairs(outfile);

    // print verbose output if -v option is specified
    if (verbose == true) {

        // read the stats bits from outfile
        fstat(outfile, &statbuf);

        // store the file size of compressed outfil
        off_t outfilesize = statbuf.st_size;

        // calculate space saving
        double ss;
        double in, out;

        in = infilesize;
        out = outfilesize;

        ss = 100 * (1 - (out / in));

        printf("Compressed file size: %lu bytes\n", (unsigned long int) out);
        printf("Uncompressed file size: %lu bytes\n", (unsigned long int) in);
        printf("Space saving: %2.2lf %% \n", ss);
    }

    close(infile);
    close(outfile);
    free(header);
    trie_delete(rootnode);

    return 0;
}
