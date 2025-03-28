#include "decrypt.h"

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
    printf("   Decrypts data using SS decryption.\n");
    printf("   Encrypted data is encrypted by the encrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./decrypt [OPTIONS]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
    printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
    printf("   -n pvfile       Private key file (default: ss.priv).\n");
}

int main(int argc, char **argv) {
    FILE *privkey = NULL;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pvfile = "ss.priv";
    bool verbose = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg != NULL) { // check for NULL optarg
                infile
                    = fopen(optarg, "r"); // open infile to decrypt based on command line argument
                if (infile == NULL) {
                    printf("Error: failed to open input decrypt file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'o':
            if (optarg != NULL) { // check for NULL optarg
                outfile
                    = fopen(optarg, "w"); // open outfile to decrypt based on command line argument
                if (outfile == NULL) {
                    printf("Error: failed to open output decrypt file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'n':
            if (optarg != NULL) { // check for NULL optarg
                privkey = fopen(
                    optarg, "r"); // open the private key file based on command line argument
                if (privkey == NULL) {
                    printf("Error: failed to open private key file. %s\n", optarg);
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

    // open private key file with default filenames if options -n is not present on the command line
    if (privkey == NULL) {
        privkey = fopen(pvfile, "r"); // open private key file with default file name
    }

    mpz_t d, pq;
    mpz_inits(d, pq, NULL); // initialize mpz_t variables d and pq to 0

    ss_read_priv(pq, d, privkey); // call function to read ss private key from private key file

    // print verbose output if -v option is specified
    if (verbose == true) {
        size_t pqtemp, dtemp;
        pqtemp = mpz_sizeinbase(pq, 10);
        dtemp = mpz_sizeinbase(d, 10);
        printf("pqtemp %zu dtemp %zu\n", pqtemp, dtemp);
        gmp_printf("pq  (%zu bits) = %Zd\n", pqtemp, pq);
        gmp_printf("d   (%zu bits) = %Zd\n", dtemp, d);
    }

    // decrypt the infile using the private key pq and d and store the decrypted output in outfile
    ss_decrypt_file(infile, outfile, d, pq);

    // close the private key file, infile, and outfile
    fclose(privkey);
    fclose(infile);
    fclose(outfile);

    mpz_clears(d, pq,
        NULL); // clear dynamically allocated memory of mpz_t variables d and pq to pass valgrind
    return 0;
}
