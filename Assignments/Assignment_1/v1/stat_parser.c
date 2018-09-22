#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	char proc_state;
	unsigned int proc_utime;
	unsigned int proc_stime;
	int proc_virtual_mem_size;
	char cmdline[1000];
} stat_statm_fields;

stat_statm_fields stat_statm_parser(int proc_id)
{
	stat_statm_fields my_fields;

	//char *proc_id_str = (char *)malloc((int)((ceil(log10(proc_id)) + 1) * sizeof(char)));
	char proc_id_str[100];
	sprintf(proc_id_str, "%d", proc_id);
	char stat_path[256] = "/proc/";
	char statm_path[256] = "/proc/";
	char cmdline_path[256] = "/proc/";
	char stat_file[7] = "/stat";
	char statm_file[8] = "/statm";
	char cmdline_file[10] = "/cmdline";
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
	
	fscanf(cmdline_file_pointer, "%[^\n]", &(my_fields.cmdline));
	fclose(cmdline_file_pointer);

	return my_fields;
}

int main1()
{
	int proc_id;
	printf("Enter the process id to get stats: ");
	scanf("%d", &proc_id);

	stat_statm_fields process_stats = stat_statm_parser(proc_id);
	printf("Proc State: %c\nProc utime: %u\nProcess stime: %u\n", process_stats.proc_state, process_stats.proc_utime, process_stats.proc_stime);
	printf("Proc virtual mem size: %d\n", process_stats.proc_virtual_mem_size);

	return 0;
}
