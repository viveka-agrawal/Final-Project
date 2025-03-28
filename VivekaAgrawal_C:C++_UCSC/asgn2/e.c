#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int ecounter = 0; // counter

double e(void) {
    double denominator = 1;
    double k = 0; // index
    double i = 0; // current calculated value of e
    while (true) {
        k += 1; // increment index
        denominator *= k; // calculate denominator with new index
        i = ((1 / denominator) + i); // calculate next term and add to current value of e
        ecounter += 1; // increment counter

        if ((1 / denominator) < EPSILON) { // check if (1 / denominator) is less than epsilon
            double j = 1 + i; // add 1 to the current calculated value of e
            return j; // return final value of e
            break;
        }
    }
}

int e_terms(void) {
    return (ecounter + 1); // return (counter + 1) to get the same number of terms as Prof Long
}
