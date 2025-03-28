#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int cmadhava = 0; // counter

double power(double a, double b) { // creating my own power function
    double c = 1;
    if (b < 0) {
        b = (-1) * b;
        a = 1 / a;
    }
    int d = 1;
    while (d <= b) {
        c *= a;
        d += 1;
    }

    return c;
}

double pi_madhava(void) {
    double denominator = 0;
    double numerator = 0;
    double k = 0; // index
    double i = 0; // current calculated value
    while (true) {
        numerator = power((-3), (-k)); // calculate numerator for new index
        denominator = ((2 * k) + 1); // calculate denominator for new index
        i = ((numerator / denominator) + i); // calculate next term and add to previous value
        double p = (numerator / denominator);
        k += 1; // increment index
        cmadhava += 1; // increment counter

        if (p < 0) { // if p has become negative
            double p = (numerator / denominator) * (-1); // making term negative
            if (p < EPSILON) {
                double j
                    = sqrt_newton(12)
                      * i; // calculate final value by multiplying square root of 12 and current calculated value
                return j;
                break;
            }
        }
    }
}

int pi_madhava_terms(void) {
    return (cmadhava - 1); // return counter - 1 to get exact terms as Prof Long
}
