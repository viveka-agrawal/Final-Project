#include "universe.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// struct Universe code given by Professor Long
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// Constructor function for creating Universe
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u
        = (Universe *) calloc(1, sizeof(Universe)); // dynamically allocate memory for universe
    u->rows = rows; // initialize universe member row variable
    u->cols = cols; // initialize universe member column variable
    // Allocating matrix example code given by Professor Long
    // dynamically memory for grid array
    bool **matrix = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        matrix[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->grid = matrix; // initialize universe member grid variable
    u->toroidal = toroidal; // initialize universe member toroidal variable
    return u; // return pointer to created Universe
}

// Destructor function for deleting Universe
void uv_delete(Universe *u) {
    // free the dynamically allocated memory for the grid array
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
    return;
}

// accessor function to return number of rows in the Universe
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// accessor function to return number of columns in the Universe
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// manipulator function to mark a cell as live at row r, column c in the Universe
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows && c < u->cols) { // check if the row and column are out of bounds
        u->grid[r][c] = true;
    }
}

// manipulator function to mark a cell as dead at row r, column c in the Universe
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows && c < u->cols) { // check if the row and column are out of bounds
        u->grid[r][c] = false;
    }
}

// accessor function to return the value of the cell at row r, column c in the Universe
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < u->rows && c < u->cols) { // check if the row and column are out of bounds
        return u->grid[r][c];
    } else {
        return false; // if row and column are out of bounds
    }
}

// manipulator function to populate the universe
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    // populate the universe initially with all dead cells
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {
            uv_dead_cell(u, i, j);
        }
    }
    // populate the live cells in the universe
    while (
        fscanf(infile, "%d %d\n", &r, &c)
        != EOF) { // while the infile has not reached the end of file, read the row-column pairs from the infile
        if (r < u->rows && c < u->cols) { // check if the row and column are out of bounds
            uv_live_cell(u, r, c); // populate live cell at row r, column c
        } else {
            return false; // if row and column are out of bounds
        }
    }
    return true; // if universe has been successfully populated
}

// accessor function to return number of live neighbor cells adjacent to row r, column c
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t numlivecells = 0; // counter for number of live neighbor cells
    // initialize temp variables for current cell row and column
    uint32_t rtemp = r;
    uint32_t ctemp = c;

    // check for neighbors on the right
    ctemp += 1; // increment column index to check for neighbor on the right
    if (u->toroidal) // check for toroidal universe
    {
        ctemp
            = ctemp
              % u->cols; // if current column index is at the right edge of the universe, then wrap around to column index 0 in the toroidal universe using modular arithmetic
    }
    if (uv_get_cell(u, r, ctemp)
        == true) { // for flat universe, function uv_get_cell() will return false if the column index is out of bounds. For toroidal universe, column index 0 will be checked by function uv_get_cell()
        numlivecells += 1; // increment number of live neighbor cells
    }

    // ----------------------------------------------------------------------------------------
    // check for neighbors on the left

    // resetting the temporary variables
    rtemp = r;
    ctemp = c;

    if (ctemp == 0) // check for column index 0 to see if left edge of universe is reached
    {
        if (u->toroidal) // check for toroidal universe
        {
            ctemp
                = u->cols
                  - 1; // if current column index is 0 at the left edge of the universe, then wrap around to column index (maximum column size - 1) in the toroidal universe
            if (uv_get_cell(u, r, ctemp)
                == true) // check if the neighbor cell at row r, column ctemp is a live cell
            {
                numlivecells += 1; // increment number of live neighbor cells
            }
        }
        // for flat universe, there is no left neighbor since the column index is 0 at the left edge of the universe
    } else if (ctemp > 0) // when column index is not at the left edge of the universe
    {
        ctemp -= 1; // decrement column index to check left neighbor
        if (uv_get_cell(u, r, ctemp)
            == true) // check if the neighbor cell at row r, column ctemp is a live cell
        {
            numlivecells += 1; // increment number of live neighbor cells
        }
    }

    // ----------------------------------------------------------------------------------------
    // check for neighbors on the top left diagonally, top, and top right diagonally

    // reset the temporary variable values
    rtemp = r;
    ctemp = c;

    uint32_t indextopneighbors
        = 2; // initialize index for top neighbors to 2 as we need to check for the top 3 neighbors

    if (rtemp == 0) // check for row index 0 to see if top edge of universe is reached
    {
        if (u->toroidal) // check for toroidal universe
        {
            rtemp
                = u->rows
                  - 1; // if current row index is 0 at the top edge of the universe, then wrap around to row index (maximum row size - 1) in the toroidal universe
            if (ctemp == 0) // check for column index 0 to see if left edge of universe is reached
            {
                // set column index to check for top left diagonal neighbor
                ctemp
                    = u->cols
                      - 1; // if current column index is 0 at the left edge of the universe, then wrap around to column index (maximum column size - 1) in the toroidal universe
            } else {
                ctemp
                    -= 1; // not at the left edge of the universe, decrement column index to check for top left diagonal neighbor
            }
            for (uint32_t i = 0; i < 3;
                 i++) { // check for top left diagonal, top, and top right diagonal neighbors
                ctemp
                    = ctemp
                      % u->cols; // if current column index is at the right edge of the universe, then wrap around to column index 0 in the toroidal universe using modular arithmetic
                if (uv_get_cell(u, rtemp, ctemp) == true) {
                    numlivecells += 1; // increment number of live neighbor cells
                }
                ctemp += 1; // increment column index
            } // end of for loop
        } // end of checking for toroidal
        // for a flat universe, there are no top neighbors beyond the top edge of the universe
    } // end of checking top edge of universe

    else if (rtemp > 0) // when row index is not at the top edge of the universe
    {
        rtemp -= 1; // decrement row index to check top neighbors
        if (ctemp == 0) // check for column index 0 to see if left edge of universe is reached
        {
            if (u->toroidal) {
                // set column index to check for top left diagonal neighbor
                ctemp
                    = u->cols
                      - 1; // if current column index is 0 at the left edge of the universe, then wrap around to column index (maximum column size - 1) in the toroidal universe
            } else {
                // for a flat universe, there are only 2 top neighbors at the left edge of the universe. There is no top left diagonal neighbor, so loop index will run from 0 to 1 to check for top and top right diagonal neighbors only
                indextopneighbors = 1;
            }
        } else {
            // not at the left edge of the universe
            // decrement column index to check for top left diagonal neighbor
            ctemp -= 1;
        }
        for (uint32_t i = 0; i <= (indextopneighbors);
             i++) { // check for top left diagonal, top, and top right diagonal neighbors
            if (u->toroidal) {
                ctemp
                    = ctemp
                      % u->cols; // if current column index is at the right edge of the universe, then wrap around to column index 0 in the toroidal universe using modular arithmetic
            }
            if (uv_get_cell(u, rtemp, ctemp) == true) {
                numlivecells += 1; // increment number of live neighbor cells
            }
            ctemp += 1; // increment column index
        } // end of for loop
    } // end of else if statement

    // ----------------------------------------------------------------------------------------
    // check for neighbors on the bottom left diagonal, bottom, and bottom right diagonal

    // resetting the temporary variables
    rtemp = r;
    ctemp = c;

    // initialize index for bottom neighbors to 2 as we need to check for the bottom 3 neighbors
    uint32_t indexbottomneighbors = 2;

    rtemp += 1; // increment row index to check for bottom neighbors
    if (u->toroidal) // check for toroidal universe
    {
        rtemp
            = rtemp
              % u->rows; // if current row index is the (maximum row size - 1) at the bottom edge of the universe, then wrap around to row index 0 in the toroidal universe using modular arithmetic
    }
    if (ctemp == 0) // check for column index 0 to see if left edge of universe is reached
    {
        if (u->toroidal) {
            // set column index to check for bottom left diagonal neighbor
            ctemp
                = u->cols
                  - 1; // if current column index is 0 at the left edge of the universe, then wrap around to column index (maximum column size - 1) in the toroidal universe
        } else {
            // for a flat universe, there are only 2 bottom neighbors at the left edge of the universe. There is no bottom left diagonal neighbor, so loop index will run from 0 to 1 to check for bottom and bottom right diagonal neighbors only
            indexbottomneighbors = 1;
        }
    } else {
        // not at the left edge of the universe
        // decrement column index to check for bottom left diagonal neighbor
        ctemp -= 1;
    }
    for (uint32_t i = 0; i <= (indexbottomneighbors);
         i++) { // check for bottom left diagonal, bottom, and bottom right diagonal neighbors
        if (u->toroidal) {
            ctemp
                = ctemp
                  % u->cols; // if current column index is at the right edge of the universe, then wrap around to column index 0 in the toroidal universe using modular arithmetic
        }
        if (uv_get_cell(u, rtemp, ctemp) == true) {
            numlivecells += 1; // increment number of live neighbor cells
        }
        ctemp += 1; // increment column index
    } // end of for loop

    return numlivecells; // return the total number of live neighbor cells
}

// function to print out the universe to outfile
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {
            if (uv_get_cell(u, i, j) == true) {
                fputc('o', outfile); // print 'o' for a live cell
            } else {
                fputc('.', outfile); // print '.' for a dead cell
            }
        }
        fprintf(outfile, "\n");
    }
}
