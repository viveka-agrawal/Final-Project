
#include "randstate.h"
#include "numtheory.h"

//#include <stdlib.h>

// Pseudocode provided by Professor Long

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, dtemp;
    mpz_inits(v, p, dtemp,
        NULL); // Initialize the NULL-terminated list of mpz_t variables v and p, and set their values to 0

    mpz_set_ui(v, 1); // set the value of the mpz_t variable v to unsigned long integer 1
    mpz_set(p, a); // setting the value of mpz_t variable p to mpz_t variable a
    mpz_set(dtemp, d); // setting the value of mpz_t variable dtemp to mpz_t variable d

    while (mpz_cmp_ui(dtemp, 0) > 0) { // run while loop if d is greater than 0
        if (mpz_odd_p(dtemp) != 0) { // check if d is odd
            mpz_mul(v, v, p); // v = v x p
            mpz_mod(v, v, n); // v = v % n
        }
        mpz_mul(p, p, p); // p = p x p
        mpz_mod(p, p, n); // p = p % n
        mpz_fdiv_q_ui(dtemp, dtemp,
            2); // d = d / 2 and check only the quotient of the division and round the quotient down
    }

    mpz_set(o, v); // set the value of mpz_t variable o to mpz_t variable v
    mpz_clears(v, p, dtemp, NULL); // free the space occupied by mpz_t variables to pass valgrind
} // end of pow_mod() function

bool is_prime(const mpz_t n, uint64_t iters) {
    // first check for base cases
    if (mpz_cmp_ui(n, 1) <= 0) {
        return false; // 0 and 1 are not prime numbers
    }
    if ((mpz_cmp_ui(n, 2) == 0) || (mpz_cmp_ui(n, 3) == 0)) {
        return true; // 2 and 3 are prime numbers
    }

    // check if n is an odd number. even number cant be prime since its divisible by 2
    if (mpz_even_p(n) != 0) // check if n is even
    {
        return false; // even number cant be prime
    }

    mpz_t r, a, y, j, nsub, notfound, two, range;
    mpz_inits(r, a, y, j, nsub, notfound, two, range,
        NULL); // Initialize the NULL-terminated list of the mpz_t variables listed in the brackets and set their values to 0
    mp_bitcnt_t s = 2;

    mpz_set(nsub, n); // store value of n in temporary variable nsub
    mpz_sub_ui(nsub, nsub, 1); // decrement nsub by 1 to store n - 1

    // find s and r such that n - 1 = 2^s * r
    while (mpz_divisible_2exp_p(nsub, s)) {
        s += 1;
    }
    s -= 1; // decrement s
    // calculate r from n - 1 and s
    mpz_tdiv_q_2exp(r, nsub, s);

    // for loop to run Miller-Rabin test iters number of times
    for (uint64_t index = 0; index < iters; index++) {

        // generate a random number between 2 and n - 2
        mpz_set(range, n);
        mpz_sub_ui(range, range, 3); // range = n - 3
        mpz_urandomm(a, state, range); // first, generate a random number between 0 and n - 4
        mpz_add_ui(a, a, 2); // next, generate a random number in the range 2 to n - 2

        pow_mod(y, a, r,
            n); // call the power-mod function passing in arguments a = base, r = exponent, and n = modulus. store the return value in y

        if ((mpz_cmp_ui(y, 1) != 0)
            && (mpz_cmp(y, nsub) != 0)) { // if y does not equal 1 and y does not equal n - 1
            mpz_set_ui(
                j, 1); // initialize the value of the mpz_t variable j to unsigned long integer 1
            while ((mpz_cmp_ui(j, s) != 1)
                   && (mpz_cmp(y, nsub)
                       != 0)) { // run while loop if j <= s - 1 and y does not equal n - 1
                mpz_set_ui(two, 2); // set mpz_t variable two to 2
                pow_mod(y, y, two,
                    n); // call the power-mod function passing in arguments y = base, 2 = exponent, and n = modulus. store the return value in y

                if (mpz_cmp_ui(y, 1) == 0) { // if y equals 1
                    mpz_clears(r, a, y, j, nsub, notfound, two, range,
                        NULL); // free the space occupied by the mpz_t variables to pass valgrind
                    return false; // n is not a prime number
                }
                mpz_add_ui(j, j, 1); // increment j
            } // end of while loop

            if (mpz_cmp(y, nsub) != 0) { // if y does not equal n - 1
                mpz_clears(r, a, y, j, nsub, notfound, two, range,
                    NULL); // free the space occupied by the mpz_t variables to pass valgrind
                return false; // n is not a prime number
            }
        }

    } // end of for loop

    mpz_clears(r, a, y, j, nsub, notfound, two, range,
        NULL); // free the space occupied by the mpz_t variables to pass valgrind

    return true; // n is a prime number
} // end of is_prime() function

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t i, primefound, numbits;
    mpz_inits(i, primefound, numbits, NULL); // initialize mpz_t variables to 0

    mpz_set_ui(numbits, 1);

    // run a while loop until a prime number is found
    while (mpz_cmp_ui(primefound, 1) != 0) {
        // run while loop until a random number with at least bits number of bits long is found
        // Generate a uniformly distributed random integer in the range 0 to 2^bits-1, inclusive. store the value of random number in i
        mpz_urandomb(i, state, bits);
        // make sure the number of bits in the generated random number is at least bits number of bits long
        mpz_add(i, i, numbits);

        // check if i is prime using Miller-Rabin algorithm running for iters number of iterations
        if (is_prime(i, iters) == true) {
            // i is a prime number based on Miller-Rabin algorithm
            mpz_set(p, i); // set the value of prime number i to mpz_t variable p
            mpz_set_ui(primefound, 1); // prime number found
        } // end of if statement
    } // end of outer while loop

    mpz_clears(i, primefound, numbits,
        NULL); // free the space occupied by the mpz_t variables to pass valgrind
} // end of make_prime() function

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, atemp, btemp;
    mpz_inits(t, atemp, btemp, NULL); // Initialize mpz_t variables to 0

    // Initialize temporary variables
    mpz_set(atemp, a);
    mpz_set(btemp, b);

    while (mpz_cmp_ui(btemp, 0) != 0) { // run while loop while btemp does not equal 0
        mpz_set(t, btemp); // set the value of mpz_t variable t to passed in argument btemp
        mpz_mod(btemp, atemp, btemp); // b = a % b
        mpz_set(atemp, t); // store the value of mpz_t variable t in atemp
    }
    mpz_set(g, atemp); // set the value of mpz_t variable g to atemp
    mpz_clears(
        t, atemp, btemp, NULL); // free the space occupied by mpz_t variables to pass valgrind
} // end of gcd() function

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, rprime, t, tprime, q, rtemp, ttemp;
    mpz_inits(r, rprime, t, tprime, q, rtemp, ttemp,
        NULL); // Initialize the NULL-terminated list of the mpz_t variables listed in the brackets and set their values to 0
    mpz_set(r, n); // set the value of mpz_t variable r to passed in argument n
    mpz_set(rprime, a); // set the value of mpz_t variable rprime to passed in argument a
    mpz_set_ui(t, 0); // initialize t to 0
    mpz_set_ui(tprime, 1); // initialize tprime to 1
    while (mpz_cmp_ui(rprime, 0) != 0) { // run while loop while rprime does not equal 0
        mpz_fdiv_q(q, r,
            rprime); // q = r / rprime and check only the quotient of the division and round the quotient down
        mpz_set(
            rtemp, r); // save the current value of r in temporary variable rtemp to be used later
        mpz_set(r, rprime); // set the value of mpz_t variable r to mpz_t variable rprime
        mpz_mul(rprime, q, rprime); // rprime = q x rprime
        mpz_sub(rprime, rtemp, rprime); // rprime = rtemp - rprime

        mpz_set(
            ttemp, t); // seve the current value of t in temporary variable ttemp to be used later
        mpz_set(t, tprime); // set the value of mpz_t variable t to mpz_t variable tprime
        mpz_mul(tprime, q, tprime); // tprime = q x tprime
        mpz_sub(tprime, ttemp, tprime); // tprime = ttemp - tprime
    }
    if (mpz_cmp_ui(r, 1) > 0) { // if r > 1
        mpz_set_ui(t, 0); // return no inverse
    }
    if (mpz_cmp_ui(t, 0) < 0) { // if t < 0
        mpz_add(t, t, n); // t = t + n
    }
    mpz_set(o, t); // set the value of mpz_t variable o to mpz_t variable t
    mpz_clears(r, rprime, t, tprime, q, rtemp, ttemp,
        NULL); // free the space occupied by the mpz_t variables to pass valgrind
} // end of mod_inverse() function
