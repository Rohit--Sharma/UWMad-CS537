#include <stdio.h>


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
	fscanf(stat_file_pointer, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %u %u", &proc_state, &utime, &stime);
	printf("Proc State: %c\nProc utime: %u\nProcess stime: %u\n", proc_state, utime, stime);

	fclose(stat_file_pointer);


	FILE* statm_file_pointer = fopen("/proc/6734/statm", "r");
	if (statm_file_pointer == NULL) {
		printf("No such file exists");
		return 0;
	}
	int proc_virt_mem_size;
	fscanf(statm_file_pointer, "%d", &proc_virt_mem_size);
	printf("\nProc virtual mem size: %d\n", proc_virt_mem_size);

	fclose(statm_file_pointer);

	return 0;
}
