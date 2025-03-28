#include "life.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep ().
#define DELAY 50000

#define OPTIONS "tsi:o:n:h" // command line options defined here

// manipulator function to populate a universe with all dead cells
void uv_populate_empty_universe(Universe *u) {
    uint32_t rows = u->rows;
    uint32_t cols = u->cols;
    for (uint32_t i = 0; i < rows; i += 1) {
        for (uint32_t j = 0; j < cols; j += 1) {
            uv_dead_cell(u, i, j); // mark the cell at row i and column j as dead
        }
    }
}

// manipulator function to generate the universe based on the 3 rules of the Game of Life
void generate(Universe *u, Universe *v) {
    uint32_t numlivecells = 0; // counter for number of live neighbor cells
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {

            // implement Rule 1

            if (uv_get_cell(u, i, j) == true) { // check if cell at row i column j is a live cell
                numlivecells = uv_census(u, i, j); // get total number of live neighbor cells

                if (numlivecells == 2
                    || numlivecells == 3) { // if number of live neighbor cells is either 2 or 3
                    uv_live_cell(v, i, j); // cell survives, mark the cell as live
                } else {
                    uv_dead_cell(v, i, j); // cell does not survive, mark the cell as dead
                }
            }

            // implement Rule 2

            if (uv_get_cell(u, i, j) == false) { // check if cell at row i column j is a dead cell
                numlivecells = uv_census(u, i, j); // get toal number of live neighbor cells

                if (numlivecells == 3) { // if the number of live neighbor cells is 3
                    uv_live_cell(v, i, j); // mark the dead cell as live cell
                } else {
                    uv_dead_cell(v, i, j); // dead cell remains dead
                }
            }
            // Rule 3: As I initialized Universe v with all dead cells, all remaining cells in universe v will be dead cells, and Rule 3 is implemented
        }
    }
}

// function to print the help message
void print_help(void) {
    printf("SYNOPSIS\n   Conway's Game of Life\n\n");
    printf("USAGE\n   ./life tsn:i:o:h\n\n");
    printf("OPTIONS\n");
    printf("  -t             Create your universe as a toroidal\n");
    printf("  -s             Silent - do not use animate the evolution\n");
    printf("  -n {number}    Number of generations [default: 100]\n");
    printf("  -i {file}      Input file [default: stdin]\n");
    printf("  -o {file}      Output file [default: stdout]\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t rows = 0;
    uint32_t cols = 0;
    bool toroidal = false;
    bool silent = false;
    uint32_t generations = 100; // default number of generations
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            toroidal = true; // set toroidal as true if -t is present on command line
            break;
        case 's':
            silent = true; // silences ncurses if -s is present on command line
            break;
        case 'n':
            if (strtoul(optarg, NULL, 10) > 100) { // handles invalid number of generations
                generations = 100;
            } else {
                generations = strtoul(optarg, NULL, 10);
            }
            break;
        case 'i':
            if (optarg != NULL) { // check for NULL optarg
                infile = fopen(optarg, "r"); // open infile based on command line argument
                if (infile == NULL) {
                    printf("Error opening %s\n", optarg);
                    return -1;
                }
            }
            break;
        case 'o':
            if (optarg != NULL) { // check for NULL optarg
                outfile = fopen(optarg, "w"); // open outfile based on command line argument
            }
            break;
        case 'h':
            print_help();

            // close the open files: infile and outfile
            fclose(infile);
            fclose(outfile);
            return 0;
            break;
        case '?': // if optarg argument is not present for -i or -o options in command line
            if (optopt == 'i' || optopt == 'o') {
                print_help(); // print the help message
                // close the open files: infile and outfile
                fclose(infile);
                fclose(outfile);
                return 0;
            }
            break;

        default: break;
        } // end of switch statement
    } // end of while loop

    // create Universes A and B
    Universe *A, *B;
    fscanf(infile, "%d %d\n", &rows,
        &cols); // read the number of rows and columns for the Universe from infile

    // create Universes A and B based on the number of rows, columns, and toroidal value
    A = uv_create(rows, cols, toroidal);
    B = uv_create(rows, cols, toroidal);

    // populate Universe A with live cells based on row-column pairs given in infile
    if (uv_populate(A, infile) == true) {
        uv_populate_empty_universe(B); // populate Universe B with all dead cells
        Universe *currgen = A; // set temporary pointer current generation to Universe A
        Universe *nextgen = B; // set temporary pointer next generation to Universe B

        // generate for the given number of generations
        for (uint32_t g = 0; g < generations; g++) {

            if (!silent) { // check if -s option is not given in command line
                initscr(); // Initialize the screen .
                curs_set(FALSE); // Hide the cursor .
                clear(); // Clear the window .
                // print all of the live cells in the current Universe
                for (int rows = 0; rows < (int) currgen->rows; rows += 1) {
                    for (int cols = 0; cols < (int) currgen->cols; cols += 1) {
                        if (uv_get_cell(currgen, rows, cols) == true) {
                            mvprintw(rows, cols, "o"); // Displays "o".
                        }
                    }
                }
                refresh(); // Refresh the window .
                usleep(DELAY); // Sleep for 50000 microseconds .
            }
            // perform one generation
            generate(currgen, nextgen);

            // swap the Universes
            if (currgen == A) {
                currgen = B;
                nextgen = A;
            } else {
                currgen = A;
                nextgen = B;
            }

            // populate the next Universe before generation with all dead cells
            uv_populate_empty_universe(nextgen);

        } // all generations completed
        endwin(); // close the screen

        // print the final generated Universe to outfile
        uv_print(currgen, outfile);
    }

    else {
        // population of Universe A failed due to invalid values in infile
        printf("Malformed input.\n"); // error message as seen in the execution of ./life_amd64
        // close the open files: infile and outfile
        fclose(infile);
        fclose(outfile);

        // call the destructor functions to free allocated memory for Universes A and B
        uv_delete(A);
        uv_delete(B);
        return 0; // end the game
    }

    // close the open files: infile and outfile
    fclose(infile);
    fclose(outfile);
    // call the destructor functions to free allocated memory for Universes A and B
    uv_delete(A);
    uv_delete(B);
    return 0;
}
