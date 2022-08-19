# System-calls-in-C
Manipulating files using system calls in C programming language

- Q1: Reverse a file using system calls.
- Q2: Keep a specific part of the file as same and reverse the rest.
- Q3: Check permissions of the output file of the above two programs using system calls. Also check that the given file contents are reversed or not.

# Assumptions Made:

- File name for all the questions are given as absolute.
- The files provided in input are text files(.txt).
- If directory is already present, then we change its permissions to 0700 else if directory is not present then we create the directory for the required permissions.

# Running Instructions:
### To run Question 1
```
gcc q1.c
./a.out < path to input file.txt>
```
### To run Question 2 
```
gcc q2.c
./a.out <path to input file.txt> <start_index> <end_index>
```
### To run Question 3
```
gcc q3.c
./a.out < oldfile > < newfile > < directory > 
```

### Output instructions
For Q1 and Q2 output will be stored in a directory named Assignment.
The output for Q3 will be printed on the terminal.

