#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#include <fcntl.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
//#include "keygen.h"
#define OPTIONS "b:i:n:d:s:vh"

// Pseudocode provided by Professor Long

void help_message(void) {
    printf("SYNOPSIS\n");
    printf("   Generates an SS public/private key pair.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./keygen [OPTIONS]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
    printf("   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
    printf("   -n pbfile       Public key file (default: ss.pub).\n");
    printf("   -d pvfile       Private key file (default: ss.priv).\n");
    printf("   -s seed         Random seed for testing.\n");
}

int main(int argc, char **argv) {
    FILE *pubkey = NULL;
    FILE *privkey = NULL;
    char *pbfile = "ss.pub";
    char *pvfile = "ss.priv";
    bool verbose = false;
    uint64_t nbits = 256;
    uint64_t iters = 50;
    uint64_t seed = time(NULL);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': nbits = strtoul(optarg, NULL, 10); break;
        case 'i': iters = strtoul(optarg, NULL, 10); break;
        case 'n':
            if (optarg != NULL) { // check for NULL optarg
                pubkey = fopen(optarg, "w"); // open public key file based on command line argument
                if (pubkey == NULL) {
                    printf("Error: failed to open public key file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'd':
            if (optarg != NULL) { // check for NULL optarg
                privkey
                    = fopen(optarg, "w"); // open private key file based on command line argument
                if (privkey == NULL) {
                    printf("Error: failed to open private key file. %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 's': seed = strtoul(optarg, NULL, 10); break;
        case 'v': verbose = true; break;
        case 'h':
            help_message();
            return 0;
            break;
        case '?': // if optarg argument is not present for -n or -d options in command line
            if (optopt == 'n' || optopt == 'd') {
                help_message(); // print the help message
                return 0;
            }
            break;
        default: break;
        } // end of switch statement
    } // end of while loop

    // open public and private key files with default filenames if options -n or -d are not present on the command line

    if (pubkey == NULL) {
        pubkey = fopen(pbfile, "w"); // open public key file with default file name
    }
    if (privkey == NULL) {
        privkey = fopen(pvfile, "w"); // open private key file with default file name
    }

    mode_t mode = 0600;

    fchmod(fileno(privkey), mode); // set permissions for private key file

    randstate_init(seed); // initialize random state variable using seed

    mpz_t p, q, n, d, pq, temp;
    mpz_inits(p, q, n, d, pq, temp, NULL); // initialize mpz_t variables to 0

    ss_make_pub(p, q, n, nbits, iters); // call function to make ss public key
    ss_make_priv(d, pq, p, q); // call function to make ss private key

    char *username = getenv("USER"); // get current username using getenv()

    ss_write_pub(n, username, pubkey); // call function to write ss public key in public key file
    ss_write_priv(pq, d, privkey); // call function to write ss private key in private key file

    // print verbose output if -v option is specified
    if (verbose == true) {
        size_t ptemp, qtemp, ntemp, pqtemp, dtemp;
        ptemp = mpz_sizeinbase(p, 10);
        qtemp = mpz_sizeinbase(q, 10);
        ntemp = mpz_sizeinbase(n, 10);
        pqtemp = mpz_sizeinbase(pq, 10);
        dtemp = mpz_sizeinbase(d, 10);

        printf("user = %s\n", username);
        gmp_printf("p  (%zu bits) =  %Zd\n", ptemp, p);
        gmp_printf("q  (%zu bits) =  %Zd\n", qtemp, q);
        gmp_printf("n  (%zu bits) =  %Zd\n", ntemp, n);
        gmp_printf("pq (%zu bits) =  %Zd\n", pqtemp, pq);
        gmp_printf("d  (%zu bits) =  %Zd\n", dtemp, d);
    }

    randstate_clear(); // clear the random state

    // close the public and private key files
    fclose(pubkey);
    fclose(privkey);

    mpz_clears(p, q, n, d, pq, temp,
        NULL); // clear dynamically allocated memory of mpz_t variables to pass valgrind
    return 0;
}
