#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1000
#define SMALL_BUFFER 10

typedef struct {
	char proc_state;
	unsigned int proc_utime;
	unsigned int proc_stime;
	int proc_virtual_mem_size;
	char cmdline[BUFFER_SIZE];
} stat_statm_cmdline_fields;

stat_statm_cmdline_fields stat_statm_cmdline_parser(int proc_id)
{
	stat_statm_cmdline_fields my_fields;

	//char *proc_id_str = (char *)malloc((int)((ceil(log10(proc_id)) + 1) * sizeof(char)));
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
		printf("No such file exists");
		return my_fields;
	}

	fscanf(stat_file_pointer, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", &(my_fields.proc_state), &(my_fields.proc_utime), &(my_fields.proc_stime));
	// printf("Proc State: %c\nProc utime: %u\nProcess stime: %u\n", my_fields.proc_state, my_fields.proc_utime, my_fields.proc_stime);

	fclose(stat_file_pointer);

	FILE* statm_file_pointer = fopen(statm_path, "r");
	if (statm_file_pointer == NULL) {
		printf("No such file exists");
		return my_fields;
	}

	fscanf(statm_file_pointer, "%d", &(my_fields.proc_virtual_mem_size));
	// printf("\nProc virtual mem size: %d\n", my_fields.proc_virtual_mem_size);

	fclose(statm_file_pointer);

	FILE* cmdline_file_pointer = fopen(cmdline_path, "r");
	if (cmdline_file_pointer == NULL) {
		printf("No such file exists");
		return my_fields;
	}
	
	fscanf(cmdline_file_pointer, "%[^\n]", my_fields.cmdline);
	fclose(cmdline_file_pointer);

	return my_fields;
}


