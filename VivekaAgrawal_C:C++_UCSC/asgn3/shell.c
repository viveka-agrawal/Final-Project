#include "shell.h"

#include "gaps.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// python pseudo given by Prof Long

// implementing shell sort algorithm as given in the pseudocode
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    for (uint32_t k = 0; k < GAPS; k++) { // run for all gaps in the range 0 to GAPS in gaps.h
        uint32_t gap
            = gaps[k]; // set gap variable equal to the gap value at the k index of the gaps array
        for (uint32_t i = gap; i < n; i++) { // for i in range gap to the length of the array
            uint32_t j = i; // store current value of i in j
            uint32_t temp = move(stats,
                A[i]); // move the array element A[i] and store the index in the temp variable
            while (
                j >= gap
                && cmp(stats, temp, A[j - gap])
                       < 0) { // while j is greater than or equal to the current gap value and temp < A[j - gap]
                A[j] = move(stats, A[j - gap]); // move A[j-gap] and store value in A[j]
                j -= gap; // decrement the current index for comparison by value of gap
            } // end of while loop
            A[j]
                = move(stats, temp); // move the array element at index temp and store value in A[j]
        } // end of inner for loop
    } // end of outer for loop
} // end of shell_sort function
