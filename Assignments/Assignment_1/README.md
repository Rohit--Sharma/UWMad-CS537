# Programming Assignment - 1
## *537ps* - A mini *ps* command

The features of the *537ps* command:
- Accepts commandline arguments for various options such as
	1. Specifying a single process id or all the current user processes
	2. Displaying the status of the process
	3. Displaying the amount of user time consumed by the process
	4. Displaying the system time consumed so far by the process
	5. Displaying the virtual memory currently being used by the process
	6. Displaying the command line arguments that were used to create the process
- Organized the source code into multiple source files to modularize the program.
- Makefile to compile and link all the source code files and create the executable was implemented.
- Graceful handling of bad input was implemented.

### Usage
The following indicates how to use the *537ps* utility. To run the command, navigate to the *src* directory and compile the program by utilizing the make utility as follows:
```
$ make
```
It executes all the source codes and creates the object files and the final executable called *537ps*.

Now, the command *537ps* can be called using various combinations of commandline arguments for the output desired. Below are a few examples to run the same along with sample outputs:
```
$ ./537ps -p <proc-id> -U- -s
<proc-id>:  S [-bash]

$ ./537ps -c
2663:   utime=113     [nautilus-desktop]
14408:  utime=55      [-bash]
22666:  utime=0       [./537ps]
...
```

### Program Organization
The program files are organized in the following manner:
- proj1/
	- README.md
	- /src/
		- 537ps_driver.c
		- 537ps_header.h
		- options_processor.c
		- proc_id_processor.c
		- proc_files_parser.c
		- Makefile
The *537ps_driver.c* file contains the main method to drive the program. Files *options_processor.c*, *proc_id_processor.c* and *proc_files_parser.c* are responsible for parsing the commandline arguments, fetching the current user's process id's, and extracting the details of the process such as virtual memory, status, etc., respectively. The *537ps_header.h* file contains the method definitions and data structures of the various methods and structures used throughtout the program.

The *Makefile* contains the rules to compile and link all the source code files and create the executable *537ps*.


### Description
To process the commandline arguments, *getopt()* was used. To fetch the proc ids from the /proc directory, *readdir()* was used and all the fetched proc ids were stored in a linked list. Details of the process were extracted by reading the corresponding *stat, statm* and *cmdline* files of the processes located in /proc/\<proc-id\> directory.

