#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "537ps_header.h"

// Returns if the process with process id "proc_id" is a user process or not
int isUserProcess(int proc_id) {
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

	int is_user_process = u_id == getuid();
	if (is_user_process) {
		return 1;
	}
	else {
		// printf("The proc id %d does not correspond to a user process\n", proc_id);
		return 0;
	}
}

struct pid_entry *return_all_processes() {
	struct pid_entry *head = NULL;
	struct pid_entry *tail = NULL;
	struct pid_entry *process_ptr = NULL;

	DIR *dir;
	struct dirent *entry;

	//int array_index = 0;
	if ((dir = opendir("/proc")) == NULL)
		perror("opendir() error");
	else {
		// puts("contents of root:");
		while ((entry = readdir(dir)) != NULL) {
			//printf("  %s\n", entry->d_name);
			if (atoi(entry->d_name) != 0) {
				if (isUserProcess(atoi(entry->d_name))) { // if (u_id == user_id) {
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
