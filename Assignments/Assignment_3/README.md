# Programming Assignment - 3
## *The MAKEFILE*

The features of the *537make* program:
- can provide filename for makefile through commandline, or it checks for makefile and Makefile
- parses through the provided makefile and identifies targets, dependencies and build rules
- It builds a directed graph between the various targets and dependencies
- It detects any cycle in the graph and exits if that's the case
- It executes these dependencies and targets in order to make the final target
- takes care of blank spaces, blank lines, comments in the makefiles and ignores them 
- if any dependency files are changed, during the next make, only the dependent/relevant targets are rebuilt 
- Makefile to compile and link all the source code files and create the executable was implemented
- Graceful handling of termination and errors was implemented.

### Usage
The following indicates how to use the *537make* utility. To run the command, compile the program by utilizing the make utility as follows:
```
$ make
```
It executes all the source codes and creates the object files and the final executable called *537make*.

To compile the program with CSA, run the following command:
```
$ scan-build make
```
Now, the command *537make* will produce the following output for certain simple testcases mentioned below:
```
$ cat file/makefile
#nnnnn
537ps: options_processor.o proc_files_parser.o proc_id_processor.o 537ps_driver.o
	gcc -Wall -o 537ps options_processor.o proc_files_parser.o proc_id_processor.o 537ps_driver.o
#alsjd


options_processor.o          : options_processor.c 537ps_header.h
	gcc -Wall -c options_processor.c

proc_files_parser.o:proc_files_parser.c 537ps_header.h
	gcc -Wall -c proc_files_parser.c

proc_id_processor.o: proc_id_processor.c   537ps_header.h
	gcc -Wall -c proc_id_processor.c

537ps_driver.o: 537ps_driver.c 537ps_header.h
	gcc -Wall -c 537ps_driver.c


$ ./537make -f file/makefile
gcc -g -Wall -Wextra -c main.c
gcc -g -Wall -Wextra -c build_spec_graph.c
gcc -g -Wall -Wextra -c text_parsing.c
gcc -g -Wall -Wextra -c build_spec_repr.c
gcc -g -Wall -Wextra -c proc_creation_prog_exe.c
gcc -g -o 537make main.o build_spec_graph.o text_parsing.o build_spec_repr.o proc_creation_prog_exe.o -lpthread 


### Program Organization
The program files are organized in the following manner:
- proj3/
	- README.md
	- partner.txt
	- build_spec_graph.c
	- build_spec_graph.h
	- build_spec_repr.c
	- build_spec_repr.h
	- main.c
	- Makefile
	- proc_creation_prog_exe.h
	- proc_creation_prog_exe.c
	- text_parsing.c
	- text_parsing.h

The *build_spec_graph.c* contains the different functions that are used for constructing and using the graph. File *build_spec_repr.c* contains the functions that actually build the graph based on various dependencies between files and targets. The *text_parsing.c* file contains the method definitions and data structures used to parse the given makefile and identify targets and dependencies. The *proc_creation_prog_exe.c* file takes care of forking the jobs (executing each of the targets). *main.c* calls each of the relevant functions from the other files to make the final target. 

The *Makefile* contains the rules to compile and link all the source code files and create the executable *537make*.


### Description
A makefile is provided through the command line or by default, the program takes the file name makefile/Makefile.
It parses through the makefile and identifies the different targets, dependencies, and rules for building the same.
A graph is creating using these targets and dependencies which is checked for cycles and then topologically sorted.
The targets are build in order from bottom-up.

###VALGRIND ISSUES AND FIXES
#ERRORS
1.
Conditional jump or move depends on uninitialised value(s)
   at 0x10A366: is_dag_cyclic (build_spec_graph.c:280)
   by 0x1094F5: main (main.c:183)
 Uninitialised value was created by a heap allocation
   at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x10B5FD: create_node (build_spec_repr.c:65)
   by 0x10B0C8: read_input_makefile (text_parsing.c:337)
   by 0x10933D: main (main.c:139)

Conditional jump or move depends on uninitialised value(s)
   at 0x109E4F: dfs_for_cycle (build_spec_graph.c:199)
   by 0x10A3EE: is_dag_cyclic (build_spec_graph.c:285)
   by 0x1094F5: main (main.c:183)
 Uninitialised value was created by a heap allocation
   at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x10B5FD: create_node (build_spec_repr.c:65)
   by 0x10B0C8: read_input_makefile (text_parsing.c:337)
   by 0x10933D: main (main.c:139)

Fix: modify_build field in every node was not being set to an initial value of 0 when it was being initialized. Fixed the code in create_node() under build_spec_graph.c and this issue was resolved.

2.
Conditional jump or move depends on uninitialised value(s)
   at 0x1097C8: main (main.c:216)
 Uninitialised value was created by a heap allocation
   at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x10A686: topo_list (build_spec_graph.c:339)
   by 0x109585: main (main.c:200)

Fix: topologically_sorted_nodes is a dynamically allocated array of adj list nodes which was not initialized to NULL. After initializing it to NULL in topo_list(), the issue was resolved.

#MEMORY LEAK ISSUES
1.
 16 bytes in 1 blocks are definitely lost in loss record 1 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10A81D: create_token (text_parsing.c:27)
    by 0x10A971: tokenize_makestring (text_parsing.c:66)
    by 0x10B5F5: create_node (build_spec_repr.c:47)
    by 0x10B1E1: read_input_makefile (text_parsing.c:369)
    by 0x10933D: main (main.c:139)

Reason: tokenize_makestring tokenizes a line with different dependencies which are assigned to nodes that are used in building the dependency graph later.
 
2.
 304 bytes in 19 blocks are definitely lost in loss record 7 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10A81D: create_token (text_parsing.c:27)
    by 0x10A971: tokenize_makestring (text_parsing.c:66)
    by 0x10B5F5: create_node (build_spec_repr.c:47)
    by 0x10B3F9: construct_graph_edges (text_parsing.c:431)
    by 0x1093BA: main (main.c:149)

Reason: This is same reason as above but while constructing the dependency graph edges between nodes.

3.
 320 bytes in 20 blocks are definitely lost in loss record 10 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10A81D: create_token (text_parsing.c:27)
    by 0x10A971: tokenize_makestring (text_parsing.c:66)
    by 0x10B5F5: create_node (build_spec_repr.c:47)
    by 0x10B129: read_input_makefile (text_parsing.c:337)
    by 0x10933D: main (main.c:139)

Reason: Same reason as 1 but at a different part of the code.

4.
 616 bytes in 1 blocks are definitely lost in loss record 16 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10A6BE: topo_list (build_spec_graph.c:339)
    by 0x109585: main (main.c:200)
Reason: topo_list nodes are used for forking processes in the correct order.

5.
 2,800 (32 direct, 2,768 indirect) bytes in 1 blocks are definitely lost in loss record 22 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x109882: create_graph (build_spec_graph.c:32)
    by 0x109378: main (main.c:144)

Reason: The created graph is used throughout the course of the program.

6.
 43,008 bytes in 42 blocks are definitely lost in loss record 26 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10AD81: read_line (text_parsing.c:196)
    by 0x10AFC4: read_input_makefile (text_parsing.c:267)
    by 0x10933D: main (main.c:139)

Reason: Read line reads makefile line by line and detects targets, dependencies and commands which it assigns accordingly and are used in the makenode function. These read buffers are directly used in creating the nodes of dependency graph, which is used throughout the course of the program.

7.
 229,952 (16 direct, 229,936 indirect) bytes in 1 blocks are definitely lost in loss record 30 of 30
    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    by 0x10B918: create_hash_table (build_spec_repr.c:141)
    by 0x109323: main (main.c:136)

Reason: Hastable holds a target and where its corresponding node is located in memory. So, it is required throughout the course of the program.

