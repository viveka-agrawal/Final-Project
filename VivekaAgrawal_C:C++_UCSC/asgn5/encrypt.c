#include "encrypt.h"

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#include <fcntl.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define OPTIONS "i:o:n:vh"

// Pseudocode provided by Professor Long

void help_message(void) {
    printf("SYNOPSIS\n");
    printf("   Encrypts data using SS encryption.\n");
    printf("   Encrypted data is decrypted by the decrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./encrypt [OPTIONS]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
    printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
    printf("   -n pbfile       Public key file (default: ss.pub).\n");
}

int main(int argc, char **argv) {
    FILE *pubkey = NULL;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pbfile = "ss.pub";
    bool verbose = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg != NULL) { // check for NULL optarg
                infile
                    = fopen(optarg, "r"); // open infile to encrypt based on command line argument
                if (infile == NULL) {
                    printf("Error: failed to open input encrypt file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'o':
            if (optarg != NULL) { // check for NULL optarg
                outfile
                    = fopen(optarg, "w"); // open outfile to encrypt based on command line argument
                if (outfile == NULL) {
                    printf("Error: failed to open output encrypt file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'n':
            if (optarg != NULL) { // check for NULL optarg
                pubkey
                    = fopen(optarg, "r"); // open the public key file based on command line argument
                if (pubkey == NULL) {
                    printf("Error: failed to open public key file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'v': verbose = true; break;
        case 'h':
            help_message();
            fclose(infile);
            fclose(outfile);
            return 0;
            break;
        case '?': // if optarg argument is not present for -i or -o options in command line
            if (optopt == 'i' || optopt == 'o') {
                help_message(); // print the help message
                return 0;
            }
            break;
        default: break;
        } // end of switch statement
    } // end of while loop

    // open public key file with default filenames if options -n is not present on the command line
    if (pubkey == NULL) {
        pubkey = fopen(pbfile, "r"); // open public key file with default file name
    }

    mpz_t n;
    mpz_init(n); // initialize mpz_t variables to 0

    char username[512]; // string to store username

    ss_read_pub(n, username, pubkey); // call function to read ss public key from public key file

    // print verbose output if -v option is specified
    if (verbose == true) {
        size_t tmp;
        printf("user = %s\n", username);
        tmp = mpz_sizeinbase(n, 10);
        gmp_printf("n (%zu bits) = %Zd\n", tmp, n);
    }

    // encrypt the infile using the public key n and store the encrypted output in outfile
    ss_encrypt_file(infile, outfile, n);

    // close the public key file, infile, and outfile
    fclose(pubkey);
    fclose(infile);
    fclose(outfile);

    mpz_clear(n); // clear dynamically allocated memory of mpz_t variables to pass valgrind
    return 0;
}
