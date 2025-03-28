# ASSIGNMENT 5: Public Key Cryptography


## Description

This assignment is implementing a cryptographic program. The program is used for encrypting messages using a public key and decrypting the coded message using private keys. We are implementing the Schmidt-Samoa Algorithm to test the primality of a number to determine if it is a prime or a composite number. The program is implemented in 3 parts: a key generator, an encryptor, and a decryptor. The key generator generates 2 keys - a public key n and 2 private keys d and pq. The key generator first generates 2 large random numbers p and q such that p does not divide q - 1 and q does not divide p - 1. The program tests if these large numbers are prime numbers using the Schmidt-Samoa Algorithm. Once p and q are determined to be prime, a public key is generated from these numbers using the formula n = p squared times q. Private keys pq and d are also calculated as follows: pq is calculated from lcm(p - 1, q - 1) and d is calculated from the inverse of n modulo pq. The encryptor then encodes the message m using public key n and produces the coded message c. The decryptor reads in the coded message c and decrypts it using the private keys d and pq to produce the original decoded message m.

## Building

Run the following to compile the keygen.c program:

$ make keygen

Run the following to compile the encrypt.c program:

$ make encrypt

Run the following to compile the decrypt.c program:

$ make decrypt

Run the following to compile the keygen.c, encrypt.c, and decrypt.c programs:

$ make or $ make all

## Running

To run the keygen.c program:

$ ./keygen

To run the encrypt.c program:

$ ./encrypt

To run the decrypt.c program:

$ ./decrypt

OPTIONS for keygen:
-b : specifies the minimum bits needed for the public modulus n.
-i : specifies the number of Miller-Rabin iterations for testing primes (default: 50).
-n pbfile : specifies the public key file (default: ss.pub).
-d pvfile : specifies the private key file (default: ss.priv).
-s : specifies the random seed for the random state initialization (default: the seconds since the
UNIX epoch, given by time(NULL)).
-v : enables verbose output.
-h : displays program synopsis and usage.

OPTIONS for encrypt:
-i : specifies the input file to encrypt (default: stdin).
-o : specifies the output file to encrypt (default: stdout).
-n : specifies the file containing the public key (default: ss.pub).
-v : enables verbose output.
-h : displays program synopsis and usage.

OPTIONS for decrypt:
-i : specifies the input file to decrypt (default: stdin).
-o : specifies the output file to decrypt (default: stdout).
-n : specifies the file containing the private key (default: ss.priv).
-v : enables verbose output.
-h : displays program synopsis and usage.
