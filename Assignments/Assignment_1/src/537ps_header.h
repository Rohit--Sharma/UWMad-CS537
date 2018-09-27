#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1000
#define SMALL_BUFFER 10
#define UIDLINENUM 8

#ifndef STRUCT_STAT_STATM_CMDLINE
#define STRUCT_STAT_STATM_CMDLINE
typedef struct {
	char proc_state;
	unsigned int proc_utime;
	unsigned int proc_stime;
	int proc_virtual_mem_size;
	char cmdline[BUFFER_SIZE];
	int ERROR_NO_SUCH_FILE;
} stat_statm_cmdline_fields;
#endif

#ifndef STRUCT_PID_ENTRY
#define STRUCT_PID_ENTRY
typedef struct pid_entry {
    int pid;
    struct pid_entry *next;
} pid_entry;
#endif

stat_statm_cmdline_fields stat_statm_cmdline_parser(int proc_id);
struct pid_entry* return_all_processes();
void options_processor(int argc, char *argv[]);
int isUserProcess(int proc_id);
