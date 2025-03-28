#!/bin/bash

make clean && make monte_carlo  #Rebuild the monte_carlo executable
./monte_carlo -n 1000 > /tmp/monte_carlo.dat  #Place the data points into a file

# This is one of the here-documents that is sent to gnuplot 
gnuplot <<END
set key autotitle columnhead  #from Instructor on piazza
set terminal pdf
set output "Figure2.pdf"
set title "Figure 2"
set xlabel "x"
set ylabel "y"
set xrange [0:1]  #makes the x axis increase by 0.2
set yrange [0:1]  #makes the y axis increase by 0.2
set object circle at 0,0 size 1  #makes a circle with the center being at the point of origin and makes the radius size 1
set size ratio -1  #from Apostolos's TA section; makes the radius size the same for x and y 
plot "/tmp/monte_carlo.dat" using 3:(column(5)==0?column(4):1/0) pt 7 ps 0.2 lc rgb "red", "/tmp/monte_carlo.dat" using 3:(column(5)==1?column(4):1/0) pt 7 ps 0.2 lc rgb "blue"
#pt 7 means pointtype 7 which indicates that its a circle and ps 0.2 means pointsize 0.2 which sets the size of the circles as 0.2
END

for i in 1 2 3 4 5  #this for loop creates 5 different files which stores data points to be graphed in the same graph
do
./monte_carlo -n 30000 > /tmp/monte_carlofig3_$i.dat
sleep 3  #this command is needed to make sure the data points are not the same when graphed
done

#This is another here-document that gets sent to gnuplot
gnuplot <<END
set key autotitle columnhead # from Instructor on piazza
set terminal pdf
set output "Figure3.pdf"
set title "Monte Carlo Error Estimation"
set ylabel "Error"
set yrange[-1:1]  #this makes the 0 axis in the center of the graph since the y axis range goes from -1 to 1
set logscale x 4  #this sets the x axis to a logarithmic scale of 4
set grid  #this creates gridlines on the plot
plot "/tmp/monte_carlofig3_1.dat" using 1:(column(2) - pi) with lines lc rgb "brown",\
     "/tmp/monte_carlofig3_2.dat" using 1:(column(2) - pi) with lines lc rgb "blue",\
     "/tmp/monte_carlofig3_3.dat" using 1:(column(2) - pi) with lines lc rgb "green",\
     "/tmp/monte_carlofig3_4.dat" using 1:(column(2) - pi) with lines lc rgb "orange",\
     "/tmp/monte_carlofig3_5.dat" using 1:(column(2) - pi) with lines lc rgb "yellow"
END

