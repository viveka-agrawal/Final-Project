#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

// pseudocode for command line arguments given by Prof Long
int main(int argc, char **argv) {
    int opt = 0;
    bool no_user_input = true;
    char argarray[10]; // array for storing command line arguments while parsing
    int i = 0; // index variable
    double calcvalue = 0; // variable to store calculated value

    bool test_a = false;
    bool test_e = false;
    bool test_b = false;
    bool test_m = false;
    bool test_r = false;
    bool test_v = false;
    bool test_n = false;
    bool test_s = false;
    bool test_h = false;

    // read and parse command line arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        no_user_input = false; // there is at least 1 argument in commmand line
        switch (opt) {
            // checking for repeated command line options and if -s is present anywhere in command line
        case 'a':
            if (!test_a) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_a = true;
            }
            break;
        case 'e':
            if (!test_e) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_e = true;
            }
            break;
        case 'b':
            if (!test_b) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_b = true;
            }
            break;
        case 'm':
            if (!test_m) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_m = true;
            }
            break;
        case 'r':
            if (!test_r) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_r = true;
            }
            break;
        case 'v':
            if (!test_v) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_v = true;
            }
            break;
        case 'n':
            if (!test_n) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_n = true;
            }
            break;
        case 's':
            if (!test_s) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_s = true;
            }
            break;
        case 'h':
            if (!test_h) {
                argarray[i] = opt; // store nonrepeated argument in array
                i++;
                test_h = true;
            }
            break;
        default: break;
        }
    } // end of while loop for reading and parsing command line arguments

    // for loop for processing nonrepeated command line arguments
    for (int j = 0; j < i; j++) {
        opt = argarray[j];
        switch (opt) {
        case 'a':
            calcvalue = e();
            printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", calcvalue, M_E,
                absolute(calcvalue - M_E));
            if (test_s) {
                printf("e terms() = %d\n", e_terms());
            }
            calcvalue = pi_euler();
            printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_euler_terms() = %d\n", pi_euler_terms());
            }
            calcvalue = pi_bbp();
            printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_bbp() terms = %d\n", pi_bbp_terms());
            }
            calcvalue = pi_madhava();
            printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_madhava() terms = %d\n", pi_madhava_terms());
            }
            calcvalue = pi_viete();
            printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_viete() terms = %d\n", pi_viete_factors());
            }
            for (double i = 0.000000000000000; i < 10.000000000000000; i += 0.100000000000000) {
                calcvalue = sqrt_newton(i);
                printf("sqrt_newton(%lf) = %16.15lf, sqrt(%lf) = %16.15lf, diff = %16.15lf\n", i,
                    calcvalue, i, sqrt(i), absolute(calcvalue - sqrt(i)));
                if (test_s) {
                    printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
                }
            }
            break; // end of case a
        case 'e':
            calcvalue = e();
            printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", calcvalue, M_E,
                absolute(calcvalue - M_E));
            if (test_s) {
                printf("e terms() = %d\n", e_terms());
            }
            break; // end of case e
        case 'b':
            calcvalue = pi_bbp();
            printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_bbp() terms = %d\n", pi_bbp_terms());
            }
            break; // end of case b
        case 'm':
            calcvalue = pi_madhava();
            printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_madhava() terms = %d\n", pi_madhava_terms());
            }
            break; // end of case m
        case 'r':
            calcvalue = pi_euler();
            printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_euler() terms = %d\n", pi_euler_terms());
            }
            break; // end of case r
        case 'v':
            calcvalue = pi_viete();
            printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", calcvalue, M_PI,
                absolute(calcvalue - M_PI));
            if (test_s) {
                printf("pi_viete() terms = %d\n", pi_viete_factors());
            }
            break; // end of case v
        case 'n':
            for (double i = 0.000000000000000; i < 10.000000000000000; i += 0.100000000000000) {
                calcvalue = sqrt_newton(i);
                printf("sqrt_newton(%lf) = %16.15lf, sqrt(%lf) = %16.15lf, diff = %16.15lf\n", i,
                    calcvalue, i, sqrt(i), absolute(calcvalue - sqrt(i)));
                if (test_s) {
                    printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
                }
            }
            break; // end of case n
        case 's': test_s = true; break; // end of case s
        case 'h':
            printf("SYNOPSIS\n   A test harness for the small numerical library.\n\n");
            printf("USAGE\n   ./mathlib-test [-aebmrvnsh]\n\n");
            printf("OPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e approximation test.\n");
            printf("  -b   Runs Bailey-Borwein-Plouffe pi approximation test.\n");
            printf("  -m   Runs Madhava pi approximation test.\n");
            printf("  -r   Runs Euler sequence pi approximation test.\n");
            printf("  -v   Runs Viete pi approximation test.\n");
            printf("  -n   Runs Newton-Raphson square root approximation tests.\n");
            printf("  -s   Print verbose statistics.\n");
            printf("  -h   Display program synopsis and usage.\n");
            break; // end of case h
        default: break;
        } // end of switch statement
    } // end of for loop

    // if no command line arguments provided, display help message
    if (no_user_input) {
        printf("SYNOPSIS\n   A test harness for the small numerical library.\n\n");
        printf("USAGE\n   ./mathlib-test [-aebmrvnsh]\n\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all tests.\n");
        printf("  -e   Runs e approximation test.\n");
        printf("  -b   Runs Bailey-Borwein-Plouffe pi approximation test.\n");
        printf("  -m   Runs Madhava pi approximation test.\n");
        printf("  -r   Runs Euler sequence pi approximation test.\n");
        printf("  -v   Runs Viete pi approximation test.\n");
        printf("  -n   Runs Newton-Raphson square root approximation tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program synopsis and usage.\n");
    }
    return 0;
} // end of main
