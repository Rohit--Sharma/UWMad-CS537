#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct pid_entry {

    int pid;
    struct pid_entry *next;

};


struct pid_entry* return_all_processes();

struct pid_entry* return_all_processes() {

    struct pid_entry* head = NULL;
    struct pid_entry* tail = NULL;
    struct pid_entry* process_ptr = NULL;
    
    DIR *dir;
    struct dirent *entry;
    
    //int array_index = 0;
    if ((dir = opendir("/proc")) == NULL)
        perror("opendir() error");
    else {
        // puts("contents of root:");
        while ((entry = readdir(dir)) != NULL) {
	    // printf("  %s\n", entry->d_name);
	    if (atoi(entry->d_name)!=0) {
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
        closedir(dir);
    }
	
    return process_ptr;
}

