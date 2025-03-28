#include "mathlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int cviete = 0; // counter

double pi_viete(void) {
    double numerator = 0;
    double k = 1; // current calculated value
    while (true) {
        numerator = sqrt_newton(2 + numerator); // calculate numerator
        k = ((numerator / 2) * k); // calculating next term and multiplying by current value
        double denominator = (numerator / 2); // calculate denominator
        cviete += 1; // increment counter

        if ((1 / denominator - 1) < EPSILON) {
            double a = (2 / k); // calculate final value
            return a;
            break;
        }
    }
}

int pi_viete_factors(void) {
    return (cviete - 1); // return counter value to match Prof Long's terms
}
