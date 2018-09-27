///////////////////////////////////////////////////////////////////////////////
//
// Description:     Extracts process details from stat, statm, cmdline files
// Course:          CS537 - Introduction to Operating Systems (Fall-2018)
// Organization:    University of Wisconsin-Madison
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      September 26, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "537ps_header.h"

/** Extracts process details from stat, statm and cmdline files 
 * 		present at the /proc/<proc-id> location.
 * 	A structure datastructure called stat_statm_cmdline_fields is 
 * 		used to store the needed statistics of the process
 *  :param proc_id: process id that is to be analyzed
 *  :return: A structure with required proc stats initialized
 */
stat_statm_cmdline_fields stat_statm_cmdline_parser(int proc_id)
{
	stat_statm_cmdline_fields my_fields;

	char proc_id_str[BUFFER_SIZE];
	sprintf(proc_id_str, "%d", proc_id);
	char stat_path[BUFFER_SIZE] = "/proc/";
	char statm_path[BUFFER_SIZE] = "/proc/";
	char cmdline_path[BUFFER_SIZE] = "/proc/";
	char stat_file[SMALL_BUFFER] = "/stat";
	char statm_file[SMALL_BUFFER] = "/statm";
	char cmdline_file[SMALL_BUFFER] = "/cmdline";
	strcat(stat_path, proc_id_str);
	strcat(stat_path, stat_file);
	strcat(cmdline_path, proc_id_str);
	strcat(cmdline_path, cmdline_file);
	strcat(statm_path, proc_id_str);
	strcat(statm_path, statm_file);

	FILE* stat_file_pointer = fopen(stat_path, "r");
	if (stat_file_pointer == NULL) {
		printf("No such process exists\n");
		my_fields.ERROR_NO_SUCH_FILE = TRUE;
		return my_fields;
	}

	// Saving necessary information from the stat file and ignoring the rest
	fscanf(stat_file_pointer, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", &(my_fields.proc_state), &(my_fields.proc_utime), &(my_fields.proc_stime));

	fclose(stat_file_pointer);

	FILE* statm_file_pointer = fopen(statm_path, "r");
	if (statm_file_pointer == NULL) {
		printf("No such process exists\n");
		my_fields.ERROR_NO_SUCH_FILE = TRUE;
		return my_fields;
	}

	// Reading virtual memory size from statm file
	fscanf(statm_file_pointer, "%d", &(my_fields.proc_virtual_mem_size));

	fclose(statm_file_pointer);

	// Extracting cmdline args of the process from cmdline file
	FILE* cmdline_file_pointer = fopen(cmdline_path, "r");
	if (cmdline_file_pointer == NULL) {
		printf("No such process exists\n");
		my_fields.ERROR_NO_SUCH_FILE = TRUE;
		return my_fields;
	}
	
	fscanf(cmdline_file_pointer, "%[^\n]", my_fields.cmdline);
	fclose(cmdline_file_pointer);

	return my_fields;
}
