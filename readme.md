Costain Nachuma
Parallel Homework 3


#Getting started

All work was done on Loni QB System.
###Data: Used facebook data from snap : https://snap.stanford.edu/data/ego-Facebook.html
###Langauges:  Python for data preprocessing 
		    C for algorithms

My implementation of BFS was a modification of online alogorithms. I has most exciting experience with coding the file processing to add edges. 

1. The program only is able to read edge list separated by single space. e.g 1 0 or 2 1
2. I used imple python script to preprocess the input file to ensure one space between nodes in the input file

#Question 1

Input file is read in and bfs is performed from the root node.

Facebook graph was used with 4039 nodes and 88234 edges

To compile : gcc serial_bfs.c queue.c graph.c -o serial.out
Run:  		 ./serial.out

#Question 2

To beat the serial program , a multiple processes are assigned a smaller graph to search. In so doing I have concurrent processes running and updating each other on whats already being visited to avoid much reptition.

Compilation:
mpicc parallel_bfs.c graph.c queue.c -o parallel.out

Run: 
mpirun -n 8 ./parallel.out ( in this case I have set 8 processes sharing the task)


#Question 3

Some of the Works of Beamers paper were done. Not trivial. 
If the graph has less to check bottom up is best in that case. frontier bitmap conversion was also utilized. To track Progress, Print statements were utilized.
Compilation: mpicc direct_optimize_bfs.c graph.c queue.c -o optimized.out

Run: mpirun -n 5 ./optimized.out








