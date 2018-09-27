///////////////////////////////////////////////////////////////////////////////
//
// Description:     Retrieves the list of user procs from /proc dir to analyze
// Course:          CS537 - Introduction to Operating Systems (Fall-2018)
// Organization:    University of Wisconsin-Madison
//
// Authors:         Rohit Kumar Sharma, M. Giri Prasanna
// Email:           rsharma@cs.wisc.edu, mugundakrish@wisc.edu
// Created on:      September 26, 2018
//
///////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "537ps_header.h"

/** Returns if the passed process id is a user process or not.
 *  :param proc_id: process id that is to be tested
 *  :return: 1 if the process id is of a user process, 0 else.
 */
int isUserProcess(int proc_id) {
	// Open /proc/<proc-id>/status file and extract uid from the 9th line
	char proc_id_str[BUFFER_SIZE];
	sprintf(proc_id_str, "%d", proc_id);
	char status_path[BUFFER_SIZE] = "/proc/";
	char status_file[BUFFER_SIZE] = "/status";
	strcat(status_path, proc_id_str);
	strcat(status_path, status_file);
	char line[BUFFER_SIZE];
	unsigned int u_id;
	FILE *status_file_pointer = fopen(status_path, "r");

	if (status_file_pointer == NULL) {
		printf("No such file exists\n");
		return 0;
	}

	int uidLineNum = UIDLINENUM;
	while (uidLineNum--) {
		fgets(line, BUFFER_SIZE, status_file_pointer);
	}

	fscanf(status_file_pointer, "%*s %d", &(u_id));
	fclose(status_file_pointer);

	// Get the user id of the current process and compare it with u_id
	int is_user_process = u_id == getuid();
	if (is_user_process) {
		return 1;
	}
	else {
		// printf("The proc id %d does not correspond to a user process\n", 
		//	proc_id);
		return 0;
	}
}

/** Returns a pointer to the head of the Linked List with all the user proc ids
 *  	Navigates to the /proc directory to read all the integer valued files
 * 		and adds them to the linked list if they correspond to the user id
 */
struct pid_entry *return_all_processes() {
	struct pid_entry *head = NULL;
	struct pid_entry *tail = NULL;
	struct pid_entry *process_ptr = NULL;

	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir("/proc")) == NULL)
		perror("opendir() error");
	else {
		while ((entry = readdir(dir)) != NULL) {
			if (atoi(entry->d_name) != 0) {
				if (isUserProcess(atoi(entry->d_name))) {
					if (head == NULL) {
						head = (struct pid_entry *)malloc(sizeof(struct pid_entry));
						tail = (struct pid_entry *)malloc(sizeof(struct pid_entry));
						process_ptr = (struct pid_entry *)malloc(sizeof(struct pid_entry));
						head->pid = atoi(entry->d_name);
						head->next = NULL;
						tail = head;
						process_ptr = head;
					}
					else {
						struct pid_entry *temp;
						temp = (struct pid_entry *)malloc(sizeof(struct pid_entry));
						temp->pid = atoi(entry->d_name);
						temp->next = NULL;
						tail->next = temp;
						tail = temp;
					}
				}
			}
		}
		closedir(dir);
	}

	return process_ptr;
}
