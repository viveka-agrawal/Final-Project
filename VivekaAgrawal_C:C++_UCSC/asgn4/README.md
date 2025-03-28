# ASSIGNMENT 4: The Game of Life


## Description

In this assignment, we are writing and implementing the Game of Life. Here are the 3 main rules of the game: 1. Any live cell with 2 or 3 neighbors survives. 2. Any dead cell ith exactly 3 live neighbors becomes a live cell. 3. All other cells die, either due to loneliness or overcrowding.

## Building

Run the following to compile the life.c program:

$ make or $ make all

## Running

To run the life.c program:

$ ./life

OPTIONS:
-t: Specify that the Game of Life is to be played on a toroidal universe.
-s: Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.
-n: Specify the number of generations that the universe goes through. The default
number of generations is 100.
-i: Specify the input file to read in order to populate the universe. By default the input
should be stdin.
-o: Specify the output file to print the final state of the universe to. By default the output
should be stdout.
