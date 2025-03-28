#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "ss.h"

#include <math.h>
#include <stdlib.h>

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {

    mpz_t notfound, ptemp, qtemp, psub, qsub;
    mpz_inits(notfound, ptemp, qtemp, psub, qsub, NULL);

    // generate a random number in the range [nbits/5, (2xnbits)/5]
    // assign number of p bits to the generated random number
    uint64_t pbits = (random() % ((2 * nbits) / 5) - (nbits / 5) + 1) + (nbits / 5);
    // calculate number of q bits using the formula n = p*p*q
    uint64_t qbits = nbits - (2 * pbits);

    while (mpz_cmp_ui(notfound, 1) != 0) {
        make_prime(p, pbits, iters); // generate a new prime number stored in p
        make_prime(q, qbits, iters); // generate a new prime number stored in q

        // check that p does not divide q - 1 and q does not divide p - 1
        mpz_sub_ui(psub, p, 1);
        mpz_sub_ui(qsub, q, 1);

        mpz_mod(ptemp, qsub, p);
        mpz_mod(qtemp, psub, q);

        if ((mpz_cmp_ui(ptemp, 0) != 0) && mpz_cmp_ui(qtemp, 0) != 0) {
            mpz_set_ui(notfound,
                1); // p and q found such that p does not divide q - 1 and q does not divide p - 1. set notfound to 1 to exit while loop
        }
    } // end of while loop

    mpz_set(ptemp, p);
    mpz_set(qtemp, q);

    // calculate the public key n
    mpz_mul(ptemp, ptemp, ptemp); // compute p*p
    mpz_mul(n, ptemp, qtemp); // compute n = p*p*q

    mpz_clears(notfound, ptemp, qtemp, psub, qsub, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t ptemp, n, psub, qsub, pqtemp, lcmtemp;
    mpz_inits(ptemp, n, psub, qsub, pqtemp, lcmtemp, NULL);

    //calculate n = p*p*q
    mpz_mul(ptemp, p, p);
    mpz_mul(n, ptemp, q);

    // calculate p - 1 and q - 1
    mpz_sub_ui(psub, p, 1);
    mpz_sub_ui(qsub, q, 1);

    // call the gcd function for p - 1 and q - 1
    gcd(pqtemp, psub, qsub);

    // calculate lcm(p - 1, q - 1)
    // lcm(p - 1, q - 1) = ((p - 1) * (q - 1)) / gcd(p - 1, q - 1)
    mpz_mul(lcmtemp, psub, qsub);
    mpz_fdiv_q(pq, lcmtemp, pqtemp);

    // call mod_inverse function to calculate d
    mod_inverse(d, n, pq);

    // clear mpz_t variables to pass valgrind
    mpz_clears(ptemp, n, psub, qsub, pqtemp, lcmtemp, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    __gmp_fprintf(pbfile, "%Zx\n", n);
    fprintf(pbfile, "%s\n", username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    fscanf(pbfile, "%s\n", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t sqrtn, ktemp, m, c;
    mpz_inits(sqrtn, ktemp, m, c, NULL);

    // calculate square root of n
    mpz_sqrt(sqrtn, n);

    // calculate log base 2 of square root n
    size_t lgsqrt = mpz_sizeinbase(sqrtn, 2);

    // subtract 1 from log base 2 of square root n
    lgsqrt = lgsqrt - 1;

    // calculate floor of log base 2 of square root n minus 1 divided by 8
    unsigned long int k = lgsqrt / 8;

    // dynamically allocate memory to store the array block that can hold k bytes
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    // starting index to store data in block array
    unsigned int blkndx = 0;

    // set the 0th block to 0xFF
    bool processblock0 = true;
    block[blkndx] = 0xFF;
    blkndx++;

    // number of bytes actually read to be stored in j
    unsigned int j, numbytestoconvert;

    // define a temporary buffer of size k
    // dynamically allocate memory to store the array buffer that can hold k bytes
    uint8_t *buffer = (uint8_t *) calloc(k, sizeof(uint8_t));

    // while loop to read unprocessed bytes from infile
    while ((j = fread(buffer, sizeof(uint8_t), k - 1, infile)) && (j != 0)) {
        // place the actual number of read bytes j into the block array staring at index 1

        for (unsigned int i = 0; i < j; i++) {
            block[blkndx] = buffer[i];
            blkndx++;
        }

        if (processblock0 == true) {
            numbytestoconvert = j + 1;
            processblock0 = false;
        } else {
            numbytestoconvert = j;
        }

        // convert the read bytes from block into mpz_t variable m
        mpz_import(m, numbytestoconvert, 1, sizeof(uint8_t), 1, 0, block);

        // reset block index to point to beginning of block array again
        blkndx = 0;

        // call the ss_encrypt function to encrypt message m using public key n
        ss_encrypt(c, m, n);

        // write the encrypted message c to outfile
        gmp_fprintf(outfile, "%Zx\n", c);

    } // end of while loop

    free(block); // free the dynamically allocated memory for the block array
    free(buffer); // free the dynamically allocated memory for the buffer array
    mpz_clears(sqrtn, ktemp, m, c, NULL); // clear mpz_t variables to pass valgrind
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // calculate log base 2 of pq
    size_t pqtemp = mpz_sizeinbase(pq, 2);

    // subtract 1 from log base 2 of pq
    pqtemp = pqtemp - 1;

    // calculate floor of log base 2 of pq minus 1 divided by 8
    unsigned long int k = pqtemp / 8;

    // dynamically allocate memory to store the array block that can hold k bytes
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    // number of bytes actually converted to be stored in j
    size_t j = 0;

    // while loop to read unprocessed bytes from infile
    while (!feof(infile)) {
        // scan in hexstring from infile
        gmp_fscanf(infile, "%Zx\n", c);

        // decrypt c back to original message m using private keys d and pq
        ss_decrypt(m, c, d, pq);

        // convert m back into bytes storing them in allocated block
        mpz_export(block, &j, 1, sizeof(block[0]), 1, 0, m);

        // write out j - 1 bytes starting from index 1 of the block to outfile
        for (unsigned int i = 1; i < j - 1; i++) {
            // write the decrypted message m to outfile
            fprintf(outfile, "%x", block[i]);
        }

        fprintf(outfile, "\n");

    } // end of while loop

    free(block); // free the dynamically allocated memory for the block array
    mpz_clears(m, c, NULL); // clear mpz_t variables to pass valgrind
}
