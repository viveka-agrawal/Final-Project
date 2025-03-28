
# ASSIGNMENT 1 - Getting Acquainted with UNIX and C


## Description

This program uses bash/shell scripting and gnuplot to create Figures 2 and 3 as shown in the assignment 1 instructions. The data for the graphs are generated from running the monte_carlo.c program, which we implement into the bash script. Figure 2 is a circle within a square and has blue dots within the circle and red dots outside of the circle. Figure 3 is a graph with 5 lines of different colors which have different data points and the lines represent the difference between the estimated value of pi calculated by the monte carlo program and pi.


## Building

Run the following to compile the monte carlo program:

$ make


The plot.sh program does not require compiling as it is a bash script.


## Running

To run the plot.sh program:

$ ./plot.sh  OR  $ sh plot.sh



To run the monte carlo program:

$ ./monte_carlo


OPTIONS: -n in the monte carlo program allows the user to choose how many iterations the monte carlo program will run  
