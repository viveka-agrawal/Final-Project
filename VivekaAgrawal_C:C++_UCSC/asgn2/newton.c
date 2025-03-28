#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

int cnewton = 0; // counter

// pseudocode provided by Prof Long
double sqrt_newton(double x) {
    double z = 0.0; // original value
    double y = 1.0; // new value
    cnewton = 0; // resetting counter
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + (x / z));
        cnewton += 1; // increment counter
    }
    return y;
}

int sqrt_newton_iters(void) {
    return cnewton; // return counter value
}
