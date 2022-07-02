Run a set of simulations using 'go run simulator.go'

You will need bash, R, Golang and a C compiler to run the whole program.

The simulator.go program will ask you questions about the sim you want to generate, for instance variables and other things ike it, such as how many copies of a sim you should generate. 

It will also ask you if you want 'var?' for some variables; in english this means do you want to vary this property, for instance if you want to vary graph size, you may enter in sequence 1, 1 and 10 for it to start at a graph size of 1, step by one each time and finish at 10. This variable is combined with any other varying property you have set, so if you have set ten steps of graph change size and ten steps of 'benefit amount' you will get 10*10 simulations each running 'numReps' times. The details for all the variables can be found in the .sh files which the C program is actually ran from.

I get my graphs and other pretty images from the R files in the main folder. Some are out of date and need to be updated to the new filesystem, having been created before the Go frontend was added on.

The data ends up in the 'plots*.csv' files. It is in the format: v1, v2, ratio, steps. R will read this CSV and output plots. There is a different 'plots*.csv' file for each type of cutting ties is initiated.
