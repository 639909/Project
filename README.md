Unit Clause Propagation in C++11
=====


Unit clause propagation algorithm for 4th year MEng project written in C++11

Makefile is provided for compilation.

The program takes in a text file as input using the command:

./UCP input.txt

The file should be CNF in the DIMACS format. For example:

c Comment line describing the file
p CNF 2 2
1 2 0
1 0 

Standard spacing should be used in the file. I.E, one space after each variable, and no spaces after the line break (0)

The program will then perform propagation on the file, and supply the output in a seperate output file (output.txt),
the output file will be in the format

c Comment line describing the file
p cnf 2 2
c This file has been propagated 
c Solution: 1

The solution line will show each solution that had been found while propagating the file.

This code is still a work in progress, and is being updated as much as possible for submission in June.

###Known Bugs

clauseVector inputs an empty element for every comment or problem line

Not every unit clause is detected on first run, multiple runs needed

