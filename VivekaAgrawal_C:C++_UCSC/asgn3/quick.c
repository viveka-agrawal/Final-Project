#include "quick.h"

#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// python pseudo given by Prof Long

// partition function places elements less than pivot into left side of array and elements greater than pivot into right side of array
int partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1; // set i equal to the value of lo minus 1
    for (uint32_t j = lo; j < hi; j += 1) { // for j in range lo to hi
        if (cmp(stats, A[j - 1], A[hi - 1]) == -1) { // check if A[j - 1] > A[hi - 1]
            i += 1; // increment i by 1
            swap(stats, &A[i - 1], &A[j - 1]); // swap A[i - 1] and A[j - 1]
        } // end of if statement
    } // end of for loop
    swap(stats, &A[i], &A[hi - 1]); // swap A[i] and A[high - 1]
    return i + 1; // return the index of the pivot
} // end of partition function

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) { // check if there are still elements left to sort
        uint32_t p = partition(stats, A, lo, hi); // calculate new pivot point
        quick_sorter(stats, A, lo, p - 1); // sort left side of array
        quick_sorter(stats, A, p + 1, hi); // sort right side of array
    } // end of if statement
} // end of quick_sorter function

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n); // start quick sorting the entire array recursively
} // end of quick_sort function
