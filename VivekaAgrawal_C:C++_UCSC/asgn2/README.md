# ASSIGNMENT 2 - A Little Slice of Pi


## Description

This program uses a C program that I used to code the different math functions and a bash/shell script to use gnuplot to graph the difference between the different math functions that I coded and the actual value of those functions from the math library as shown in the assignment 2 instructions. These functions include the e function, the madhava series, the Bailey-Borwein-Plouffe formula, Viète’s formula, Euler’s Solution, and the Newton-Raphson method. The functions are coded withing their own C file and are run by mathlib-test. 

## Building

Run the following to compile the mathlib-test.c program:

$ make or $ make all

## Running

To run the mathlib-test.c program:

$ ./mathlib-test

OPTIONS:

-a: Runs all tests
-e: Runs e approximation test
-b: Runs Bailey-Borwein-Plouffeπ approximation test
-m: Runs Madhava pi approximation test
-r: Runs Euler sequence pi approximation test
-v: Runs Viète pi approximation test
-n: Runs Newton-Raphson square root approximation tests
-s: Enable printing of statistics to see computed terms and factors for each tested function
-h: Display a help message detailing program usage
