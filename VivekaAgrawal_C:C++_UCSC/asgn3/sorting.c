#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "ahbsqr:n:p:H" // command line options defined here
#define BIT_MASK                                                                                   \
    0x3FFFFFFF // defining 30 bit bit-mask for pseudorandom number as specified in assignment

typedef enum {
    SHELL,
    BATCHER,
    HEAP,
    QUICK
} Sorts; // define enumeration of the 4 sorting methods; SHELL = 0, BATCHER = 1, HEAP = 2, QUICK = 3

int main(int argc, char **argv) {
    int opt = 0;
    Set s = set_empty(); // create a new set s with empty values
    Stats stats; // create variable stats of type Stats
    reset(&stats); // resets moves and compares values in stats
    uint32_t seed = 13371453; // random number seed as stated in assignment
    uint32_t size = 100; // default size of array
    uint32_t element = 100; // default number of elements
    int no_user_input = true; // variable to check for no user input
    int test_H = false; // variable to check for -H option

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        no_user_input = false; // at least 1 command line argument is passed in command line
        switch (opt) {
        case 'a': s = set_universal(); break; // set all elements in set s to 1 for option -a
        case 'h': s = set_insert(s, HEAP); break; // set element for heap sort in set s
        case 'b': s = set_insert(s, BATCHER); break; // set element for batcher sort in set s
        case 's': s = set_insert(s, SHELL); break; // set element for shell sort in set s
        case 'q': s = set_insert(s, QUICK); break; // set element for quick sort in set s
        case 'r':
            seed = strtoul(optarg, NULL,
                10); // store the seed given in command line argument as an unsigned long integer at base 10
            break;
        case 'n':
            if (strtoul(optarg, NULL, 10)
                > 100) { // check if the array size given in command line is greater than 100
                size = 100; // set array size to 100
            } else {
                size = strtoul(optarg, NULL,
                    10); // else set array size to the value specified in the command line
            }
            break;
        case 'p':
            if (strtoul(optarg, NULL, 10)
                > 100) { // check if the element size given in command line is greater than 100
                element = 100; // set element size to 100
            } else {
                element = strtoul(optarg, NULL,
                    10); // else set element size to the value specified in the command line
            }
            break;
        case 'H': test_H = true; break; // -H is present in the command line
        default: break;
        } // end of switch statement
    } // end of while loop
    srandom(seed); // set the random number seed
    uint32_t *A = (uint32_t *) calloc(
        size, sizeof(uint32_t)); // dynamically allocate memory to store the array A
    for (uint32_t i = 0; i < size; i += 1) { // for i in range 0 to the size of the array
        A[i]
            = random()
              & BIT_MASK; // initialize the elements of the array by generating the pseudorandom number and fitting in 30 bits
    }
    if (size < element) { // if size of array < specified number of elements,
        element = size; // set number of elements to the size of the array
    }
    if (test_H
        || no_user_input) { // if either -H is specified in command line or there is no user input
        printf("Select at least one sort to perform\n");
        printf("SYNOPSIS\n");
        printf("  A collection of comparison-based algorithms.\n\n");
        printf("USAGE\n");
        printf("  ./sorting [-Hasbhqn:p:r] [-n length] [-p elements] [-r seed]\n\n");
        printf("OPTIONS\n");
        printf("  -H		 Display program help and usage.\n");
        printf("  -a		 Enable all sorts.\n");
        printf("  -s             Enable Shell Sort.\n");
        printf("  -b             Enable Batcher Sort.\n");
        printf("  -h             Enable Heap Sort.\n");
        printf("  -q             Enable Quick Sort.\n");
        printf("  -n length      Specify number of array elements (default: 100).\n");
        printf("  -p elements    Specify number of elements to print (default: 100).\n");
        printf("  -r seed        Specify random seed (default: 13371453).\n");
    }
    // check for -a option in command line
    if (set_member(s, SHELL) && set_member(s, BATCHER) && set_member(s, HEAP)
        && set_member(s, QUICK)) { // check if all sorting methods are specified in set s
        reset(&stats);
        shell_sort(
            &stats, A, size); // call the shell sort function to sort array A of length value size
        printf("Shell Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the elements of the sorted array
        int j = 0;
        for (uint32_t i = 0; i < element;
             i += 1) { // for i in range 0 to the number of elements to print
            printf("%13" PRIu32, A[i]); // printing array element with a width of 13
            j += 1;
            if (j > 4) { // check for printing the table in 5 columns
                j = 0;
                printf("\n");
            }
        }
        reset(&stats);
        srandom(seed); // initialize the random seed again
        for (uint32_t i = 0; i < size; i += 1) {
            A[i]
                = random()
                  & BIT_MASK; // initialize the elements of the array by generating the pseudorandom number and fitting in 30 bits
        }
        batcher_sort(
            &stats, A, size); // call the batcher sort function to sort array A of length value size
        printf("Batcher Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print out the sorted array in a table with 5 columns
        int k = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            k += 1;
            if (k > 4) {
                k = 0;
                printf("\n");
            }
        }
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        heap_sort(&stats, A,
            size); // call the function heap sort function to sort array A of length value size
        printf("Heap Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int l = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            l += 1;
            if (l > 4) {
                l = 0;
                printf("\n");
            }
        }
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        quick_sort(
            &stats, A, size); // call the function quick sort to sort array A with length value size
        printf("Quick Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int m = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            m += 1;
            if (m > 4) {
                m = 0;
                printf("\n");
            }
        }
        s = set_empty(); // clear the elements in set s
    } // end of -a command line option
    // check for option -s
    if (set_member(s, SHELL)) {
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        shell_sort(
            &stats, A, size); // call the function shell sort to sort array A with length value size
        printf("Shell Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int j = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            j += 1;
            if (j > 4) {
                j = 0;
                printf("\n");
            }
        }
        printf("\n");
    } // end of -s command line option
    // check for -b command line option
    if (set_member(s, BATCHER)) {
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        batcher_sort(
            &stats, A, size); // call batcher sort function to sort array A of length value size
        printf("Batcher Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int j = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            j += 1;
            if (j > 4) {
                j = 0;
                printf("\n");
            }
        }
        printf("\n");
    } // end of -b command line option
    // check for -h command line option
    if (set_member(s, HEAP)) {
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        heap_sort(
            &stats, A, size); // call the heap sort function to sort array A of length value size
        printf("Heap Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int j = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            j += 1;
            if (j > 4) {
                j = 0;
                printf("\n");
            }
        }
        printf("\n");
    } // end of -h command line option
    // check for -q command line option
    if (set_member(s, QUICK)) {
        reset(&stats);
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = random() & BIT_MASK;
        }
        quick_sort(
            &stats, A, size); // call quick sort function to sort array A with length value size
        printf("Quick Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", element,
            stats.moves, stats.compares);
        // print the sorted array in a table with 5 columns
        int j = 0;
        for (uint32_t i = 0; i < element; i += 1) {
            printf("%13" PRIu32, A[i]);
            j += 1;
            if (j > 4) {
                j = 0;
                printf("\n");
            }
        }
        printf("\n");
    } // end of -q command line option

    free(A); // free the dynamically allocated memory for array A in order to pass valgrind

    return 0;
} // end of main
