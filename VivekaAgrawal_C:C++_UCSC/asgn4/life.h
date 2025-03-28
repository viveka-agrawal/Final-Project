#include "universe.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// manipulator function to populate a universe with all dead cells
void uv_populate_empty_universe(Universe *u);

// manipulator function to generate the universe based on the 3 rules of the Game of Life
void generate(Universe *u, Universe *v);

// function to print the help message
void print_help(void);
