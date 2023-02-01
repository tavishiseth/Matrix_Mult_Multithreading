# Matrix_Mult_Multithreading

Problem Statements:
This assignment requires you to multiply arbitrarily large matrices using parallelism provided by the Linux process and threads libraries.

Write a C program P1 which takes three filenames (in1.txt, in2.txt and out.txt) as command line arguments. The first two files (in1.txt and in2.txt) contain two matrices of arbitrary size but satisfying the criteria for matrix multiplication. The sizes will be passed in the command line. P1 spawns n threads which each read row(s) and column(s) each from in1.txt and in2.txt.
Different threads should read different parts of the file. Vary the number of threads from 1… to arbitrarily large.
Record the time that it takes to read the entire file into memory with different number of threads (1, 2, … n). The timing should be at the granularity of nanoseconds.
Plot time against the number of threads for different input sizes. Analyse the findings from the plots.

Write a C program (P2) which uses IPC mechanisms to receive the rows and columns read by P1. P2 spawns multiple threads to compute the cells in the product matrix. The program stores the product matrix in the file out.txt
Vary the number of threads from 1… to arbitrarily large.
Record the time it takes to compute the product with different number of threads.  The timing should be at the granularity of nanoseconds.
Plot the time against the number of threads for different input sizes. Analyse the findings from the plots.

<img width="414" alt="image" src="https://user-images.githubusercontent.com/75008683/215978737-e2a71b51-4c22-46c7-bd48-e9db2b90f2c3.png">


Using high level maths libraries is not allowed. It will be ensured in the test cases that each element of the product matrix fits into a long long int
Sample input, output and a python script to generate and multiply matrices is here.
How to run: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt
Where in1.txt contains a ixj matrix, in2.txt contains a jxk matrix and the ixk product matrix is stored in out.txt

50 20 50 in1.txt in2.txt out.txt
