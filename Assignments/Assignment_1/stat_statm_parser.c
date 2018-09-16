#include <stdio.h>

typedef struct {
	char proc_state;
	unsigned int proc_utime;
	unsigned int proc_stime;
	int proc_virtual_mem_size;
} stat_statm_fields;

int main()
{
	FILE* stat_file_pointer = fopen("/proc/6734/stat", "r");
	if (stat_file_pointer == NULL) {
		printf("No such file exists");
		return 0;
	}

	int pid;
	char proc_name[100];
	int sess_id;
	char proc_state;
	unsigned int utime;
	unsigned int stime;

	stat_statm_fields my_fields;

	fscanf(stat_file_pointer, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", &(my_fields.proc_state), &(my_fields.proc_utime), &(my_fields.proc_stime));
	printf("Proc State: %c\nProc utime: %u\nProcess stime: %u\n", my_fields.proc_state, my_fields.proc_utime, my_fields.proc_stime);

	fclose(stat_file_pointer);


	FILE* statm_file_pointer = fopen("/proc/6734/statm", "r");
	if (statm_file_pointer == NULL) {
		printf("No such file exists");
		return 0;
	}
	int proc_virt_mem_size;
	fscanf(statm_file_pointer, "%d", &(my_fields.proc_virtual_mem_size));
	printf("\nProc virtual mem size: %d\n", my_fields.proc_virtual_mem_size);

	fclose(statm_file_pointer);

	return 0;
}
