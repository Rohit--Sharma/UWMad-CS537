# CS 537: Introduction to Operating Systems
## University of Wisconsin-Madison
### Fall 2018 - Programming Assignments

This repository consists of the programming assignments that were implemented as a part of CS537 course. They are written in `C` and targeted to run on UWMadison *Linux* workstations. 

Below are details of individual assignments:

1. `537ps`: Implementation of an interesting and important Linux Utility, `ps`.

	i. The code is scanned for style and security mistakes with a tool *Clang Static Analyzer (CSA)*.

2.  **Synchronizing with Threads and Shared Memory**: A simple synchronized queue problem is implemented using synchronized threads.

3. `537make`: Implementation of the *Make* utility which starts new programs on UNIX-type operating systems.

	i. Problems with dynamic memory allocation and pointers were checked with a tool *Valgrind* and fixed.

4. `malloc` library: A safe malloc/free library is implemented which does all the memory safety checks before allocating/freeing/accessing dynamic memory and ensures that the program behaves well.
