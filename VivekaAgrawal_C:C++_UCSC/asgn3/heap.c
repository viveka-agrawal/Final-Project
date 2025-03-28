#include "heap.h"

#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// python pseudo given by Prof Long

// finding the max child
int max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first; // calculate the index of the left child
    uint32_t right = left + 1; // calculate the index of the right child
    if (right <= last
        && cmp(stats, A[right - 1], A[left - 1]) > 0) { // check if right child is the max child
        return right; // return the index of the right child as the right child is the max child
    } // end of if statement
    return left; // return the index of the left child as the left child is the max child
} // end of max_child function

// fix the heap by removing elements from the top of the heap and storing at the end of the array
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t found = false; // set found equal to false
    uint32_t mother = first; // store value of first index in variable mother
    uint32_t great = max_child(stats, A, mother, last); // calling max_child function

    while (mother <= last / 2
           && !(found)) { // while mother is less than or equal to last divided by 2 and not found
        if (cmp(stats, A[mother - 1], A[great - 1]) < 0) { // if A[mother - 1] < A[great - 1]
            swap(stats, &A[mother - 1], &A[great - 1]); // swap A[mother - 1] and A[great - 1]
            mother = great; // set value of mother equal to the value of great
            great = max_child(stats, A, mother, last); // calling max_child function
        } // end of if statement
        else {
            found = true; // set found equal to true
        } // end of else statement
    } // end of while loop
} // end of fix_heap function

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = last / 2; father > first - 1;
         father -= 1) { // build the max heap starting from array index first to array index last
        fix_heap(stats, A, father, last); // fix the heap again in for loop
    } // end of for loop
} // end of build_heap function

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1; // set first index of the array to 1
    uint32_t last = n; // set last index of the array to the length of the array
    build_heap(stats, A, first, last); // build the heap in the range first to last

    for (uint32_t leaf = last; leaf > first;
         leaf -= 1) { // remove elements from the top of the heap and store at the end of the array
        swap(stats, &A[first - 1], &A[leaf - 1]); // swap the first element and leaf element
        fix_heap(stats, A, first, leaf - 1); // fix the heap again in for loop
    } // end of for loop
} // end of heap_sort function
