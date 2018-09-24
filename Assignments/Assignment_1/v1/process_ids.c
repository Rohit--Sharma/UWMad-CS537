#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define UIDLINENUM 8

struct pid_entry {
    int pid;
    struct pid_entry *next;
};

struct pid_entry* return_all_processes();

struct pid_entry* return_all_processes() {
    struct pid_entry* head = NULL;
    struct pid_entry* tail = NULL;
    struct pid_entry* process_ptr = NULL;
    
    int user_id = getuid();

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
			char proc_id_str[BUFFER_SIZE];
			sprintf(proc_id_str, "%d", atoi(entry->d_name));
			char status_path[BUFFER_SIZE] = "/proc/";
			char status_file[BUFFER_SIZE] = "/status";
			strcat(status_path, proc_id_str);
			strcat(status_path, status_file);
			//printf("%s\n", status_path);
			char line[BUFFER_SIZE];
			int u_id;
			
			FILE* status_file_pointer = fopen(status_path, "r");
			if (status_file_pointer == NULL) {
				printf("No such file exists");
				return 0;
			}

			int uidLineNum = UIDLINENUM;
			while (uidLineNum--) {
				fgets(line, BUFFER_SIZE, status_file_pointer);
			}

			fscanf(status_file_pointer, "%*s %d", &(u_id));
			fclose(status_file_pointer);
			//printf("UID is %d\n", u_id);
			
			if (u_id==user_id) {
				if (head == NULL) {
				        head = (struct pid_entry*)malloc(sizeof(struct pid_entry));
				        tail = (struct pid_entry*)malloc(sizeof(struct pid_entry));
			 	        process_ptr = (struct pid_entry*)malloc(sizeof(struct pid_entry));
				    head->pid = atoi(entry->d_name);
				    head->next = NULL;
				    tail = head;
				    process_ptr = head;
				}
				else {
				    struct pid_entry* temp;
				    temp = (struct pid_entry*)malloc(sizeof(struct pid_entry));
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

