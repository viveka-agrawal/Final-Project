#include "batcher.h"

#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// python pseudo given by Prof Long

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y])
        == 1) { // checking if A[x] > A[y]; checking if return value from cmp function is equal to 1 to indicate x > y
        swap(stats, &A[y], &A[x]); // if A[x] > A[y], swap array values A[x] and A[y]
    } // end of if statement
} // end of comparator function

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n
        == 0) { // if the length of array A is 0, there are no array elements so return from the function
        return;
    } // end of if statement

    uint32_t t = log2(n) + 1; // calculating bit-length of array size n
    uint32_t p = 1 << (t - 1); // calculate the initial value of k-sort

    while (p > 0) {
        uint32_t q = 1 << (t - 1); // storing the current value of k-sort
        uint32_t r = 0; // set the initial partition for comparison equal to 0
        uint32_t d = p; // set the gap between the indices for comparison

        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i++) { // for i in range 0 to (n-d)
                if ((i & p)
                    == r) { // check if partition that i falls into is eligible for comparison
                    comparator(stats, A, i,
                        i + d); // calling the comparator function to compare array elements A[i] and A[i+d] and swap if A[i] > A[i+d]
                } // end of if statement
            } // end of for loop

            d = q - p; // calculate the new value of gap between indices for comparison
            q >>= 1; // decrement the current value of k-sort in the inner loop by 2
            r = p; // set the new partition for comparison
        } // end of inner while loop

        p >>= 1; // decrement the value of k-sort in the outer loop by 2
    } // end of outer while loop
} // end of function
