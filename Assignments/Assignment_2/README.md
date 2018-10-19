# Programming Assignment - 2
## *Shared Memory Producer/Consumer Program*

The features of the *prodcomm* program:
- Reads lines from stdin, processes and outputs the result to stdout.
- The different threads are synchronized using semaphores.
- Buffer Size and Queue Size can be changed from the producer_consumer_driver.c
- Queue structures have fields for statistics to track enqueue, dequeue, and respective blocking counts. 
- Makefile to compile and link all the source code files and create the executable was implemented.
- Graceful handling of termination and errors was implemented.

### Usage
The following indicates how to use the *prodcomm* utility. To run the command, compile the program by utilizing the make utility as follows:
```
$ make
```
It executes all the source codes and creates the object files and the final executable called *537ps*.

To compile the program with CSA, run the following command:
```
$ scan-build make
```
Now, the command *prodcomm* will produce the following output for certain simple testcases mentioned below:
```
$ cat test_input1
hey this is a test
end of test file

$ ./prodcomm < test_input1
HEY*THIS*IS*A*TEST
END*OF*TEST*FILE


Queue Statistics:

Queue 1 (Reader to Munch1):
1. Enqueue Count: 2
2. Dequeue Count: 2
3. Enqueue Block Count: 0
4. Dequeue Block Count: 1

Queue 2 (Munch1 to Munch2):
1. Enqueue Count: 2
2. Dequeue Count: 2
3. Enqueue Block Count: 0
4. Dequeue Block Count: 1

Queue 3 (Munch2 to Writer):
1. Enqueue Count: 2
2. Dequeue Count: 2
3. Enqueue Block Count: 0
4. Dequeue Block Count: 1
```

### Program Organization
The program files are organized in the following manner:
- proj2/
	- README.md
	- partner.txt
	- code/
		- producer_consumer_header.h
		- producer_consumer_driver.c
		- thread.c
		- queue.c
		- Makefile

The *producer_consumer_driver.c* creates the three queues and the multiple threads. File *queue.c* contains the queue function definitions for creating a queue, enqueueing a string, dequeueing a string, and printing queue statistics. *thread.c* contains the definitions for the Reader, Munch1, Munch2, and Writer processes. The *producer_consumer_header.h* file contains the method definitions and data structures of the various methods and structures used throughtout the program.

The *Makefile* contains the rules to compile and link all the source code files and create the executable *prodcomm*.


### Description
    The Reader thread reads from standard input, one line at a time. Reader takes each line of the input and passes it to thread Munch1 through a queue of character strings.
    Munch1 scans the line and replaces each space character (not tabs or newlines) with an asterisk ("*") character. It then passes the line to thread Munch2 through another queue of character strings.
    Munch2 scans the line and converts all lower case letters to upper case (e.g., convert "a" to "A"). It then passes the line to thread Writer though yet another queue of character strings.
    Writer writes the line to standard output.
