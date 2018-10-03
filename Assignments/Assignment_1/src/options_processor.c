///////////////////////////////////////////////////////////////////////////////
//
// Description:     Processing commandline arguments
// Course:          CS537 - Introduction to Operating Systems (Fall-2018)
// Organization:    University of Wisconsin-Madison
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      September 26, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "537ps_header.h"

/** Commandline arguments processing and handling bad input gracefully.
 *  After fetching the commandline arguments, it calls the other utility 
 *  	functions and extract the required details.
 *  :param argc: number of commandline arguments passed
 *  :param argv: array of all the argc commandline arguments
 */
void options_processor(int argc, char *argv[])
{
	// External variables used by getopt()
	extern char *optarg;
	extern int optind;
	extern int optopt;

	// Configuration flags set with their default values
	int get_all_proc_ids = TRUE;
	int proc_id = 0;
	int get_state = FALSE;
	int get_utime = TRUE;
	int get_stime = FALSE;
	int get_vmem = FALSE;
	int get_cmdline = TRUE;

	int c;
	// Iterating through each argument by using getopt() to handle
	// p: indicates that p needs a value argument to follow
	// U:: indicates that we can also pass argument as -U-
	while ((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1)
		switch (c)
		{
			case 'p':
				get_all_proc_ids = FALSE;
				if (optarg != NULL) {
					proc_id = atoi(optarg);
					if (proc_id == 0) {
						fprintf(stderr, "Option -p requires a valid proc id.\n");
						return;
					}
				}
				break;
			case 's':
				get_state = TRUE;
				if (optarg != NULL) {
					if (strcmp(optarg, "-") == 0) {
						get_state = TRUE;
					}
					else {
						printf("Unknown option passed to s\n");
						return;	
					}
				}
				break;
			case 'U':
				// Check if -U- was passed. 
				// In such a case, optarg will be set to '-'
				if (optarg != NULL) {
					if (strcmp(optarg, "-") == 0) {
						get_utime = FALSE;
					}
					else {
						printf("Unknown option passed to U\n");
						return;	
					}
				}
				else {
					get_utime = TRUE;
				}
				break;
			case 'S':
				get_stime = TRUE;
				if (optarg != NULL) {
					if (strcmp(optarg, "-") == 0) {
						get_stime = TRUE;
					}
					else {
						printf("Unknown option passed to S\n");
						return;	
					}
				}
				break;
			case 'v':
				get_vmem = TRUE;
				if (optarg != NULL) {
					if (strcmp(optarg, "-") == 0) {
						get_vmem = TRUE;
					}
					else {
						printf("Unknown option passed to v\n");
						return;	
					}
				}
				break;
			case 'c':
				if (optarg != NULL) {
					if (strcmp(optarg, "-") == 0) {
						get_cmdline = FALSE;
					}
					else {
						printf("Unknown option passed to c\n");
						return;	
					}
				}
				else {
					get_cmdline = TRUE;
				}
				break;
			case '?':	// When an unknown option is encountered, this case gets triggered
				if (optopt == 'p')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return;
			default:
				abort();
				return;
		}

	///
	/// The scope from line 80 to line 84 checks if any non-option arguments are passed.
	/// 	For ex: ./537ps -p 37 -s foo -U. Here 'foo' is a non-option arg that will be
	///     caught in the below code.
	int index;
	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
	if (argc > optind)
		return;
	///

	// If get_all_proc_ids is set to true, get all the proc ids of the current user as a linked list
	if (get_all_proc_ids) {
		pid_entry *head = return_all_processes();

		// For each returned process id, extract the required details based on the passed flags
		while (head != NULL) {
			stat_statm_cmdline_fields proc_info = stat_statm_cmdline_parser(head->pid);
			if (proc_info.ERROR_NO_SUCH_FILE == FALSE) {
				printf("%d:\t", head->pid);
				if (get_state) {
					printf("%c  ", proc_info.proc_state);
				}
				if (get_utime) {
					printf("utime=%u\t", proc_info.proc_utime);
				}
				if (get_stime) {
					printf("stime=%u\t", proc_info.proc_stime);
				}
				if (get_vmem) {
					printf("vmemory=%d\t", proc_info.proc_virtual_mem_size);
				}
				if (get_cmdline) {
					printf("[%s]", proc_info.cmdline);
				}
				printf("\n");

				head = head->next;
			}
		}
	}
	// If get_all_proc_ids is set to false, only get the specific process id passed with -p flag
	else {
		// For the passed process id, extract the required details based on the passed flags
		stat_statm_cmdline_fields proc_info = stat_statm_cmdline_parser(proc_id);
		if (proc_info.ERROR_NO_SUCH_FILE == FALSE) {
			printf("%d:\t", proc_id);
			if (get_state) {
				printf("%c  ", proc_info.proc_state);
			}
			if (get_utime) {
				printf("utime=%u\t", proc_info.proc_utime);
			}
			if (get_stime) {
				printf("stime=%u\t", proc_info.proc_stime);
			}
			if (get_vmem) {
				printf("vmemory=%d\t", proc_info.proc_virtual_mem_size);
			}
			if (get_cmdline) {
				printf("[%s]", proc_info.cmdline);
			}
			printf("\n");
		}
		else {
			return;
		}
	}
}
