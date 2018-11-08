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
