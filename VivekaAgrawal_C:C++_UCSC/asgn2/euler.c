#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int ceuler = 0; // counter

double pi_euler(void) {
    double y = 0; // next term
    double k = 2; // index
    double total = 1; // current total calculated value
    double squared;
    while (true) {
        squared = k * k; // denominator is squared
        y = (1 / squared); // calculate next term based on formula
        total += y; // add next term value to current total
        k += 1; // increment index
        ceuler += 1; // increment counter
        if (y < EPSILON) {
            break;
        }
    }
    total *= 6; // multiply current total by 6 based on formula
    double final = sqrt_newton(total); // taking square root of current total
    return final; // return final value
}

int pi_euler_terms(void) {
    return ceuler; // return counter value
}
