#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int cbbp = 0; // counter

double power_bbp(double a, double b) { // creating my own power function
    double c = 1;
    if (b < 0) {
        b = (-1) * b;
        a = 1 / a;
    }
    int d = 1;
    while (d <= b) {
        c = c * a;
        d += 1;
    }
    return c;
}

double pi_bbp(void) {
    double denominator = 0;
    double numerator = 0;
    double k = 0; // index
    double i = 0; // current calculated value
    while (true) {
        numerator = power_bbp(16, -k)
                    * (k * (120 * k + 151) + 47); // calculate numerator based on formula
        denominator = k * (k * (k * (512 * k + 1024) + 712) + 194)
                      + 15; // calculate denominator based on formula
        i = ((numerator / denominator) + i); // calculate next term and add to current value
        double p = (numerator / denominator);
        k += 1; // increment index
        cbbp += 1; // increment counter

        if (p < EPSILON) {
            return i; // return final calculated value
            break;
        }
    }
}

int pi_bbp_terms(void) {
    return cbbp; // return counter value
}
