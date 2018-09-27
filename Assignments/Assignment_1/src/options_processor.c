#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "537ps_header.h"

void options_processor(int argc, char *argv[])
{

	extern char *optarg;
	extern int optind;
	extern int optopt;

	int get_all_proc_ids = TRUE;
	int proc_id = 0;
	int get_state = FALSE;
	int get_utime = TRUE;
	int get_stime = FALSE;
	int get_vmem = FALSE;
	int get_cmdline = TRUE;

	int c;

	while ((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1)
		switch (c)
		{
			case 'p':
				get_all_proc_ids = FALSE;
				proc_id = atoi(optarg);
				if (proc_id == 0) {
					fprintf(stderr, "Option -p requires a valid proc id.\n");
					return;
				}
				break;
			case 's':
				get_state = TRUE;
				break;
			case 'U':
				if (optarg != NULL)
					get_utime = ((strcmp(optarg, "-") == 0) ? FALSE : TRUE);
				break;
			case 'S':
				get_stime = TRUE;
				break;
			case 'v':
				get_vmem = TRUE;
				break;
			case 'c':
				if (optarg != NULL)
					get_cmdline = ((strcmp(optarg, "-") == 0) ? FALSE : TRUE);
				break;
			case '?':
				if (optopt == 'p')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return;
			default:
				abort();
				return;
		}

	int index;
	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
	if (argc > optind)
		return;

	if (get_all_proc_ids) {
		pid_entry *head = return_all_processes();
		while (head != NULL) {
			stat_statm_cmdline_fields proc_info = stat_statm_cmdline_parser(head->pid);
				printf("%d: ", head->pid);
			if (get_state) {
				printf("%c ", proc_info.proc_state);
			}
			if (get_utime) {
				printf("utime=%u ", proc_info.proc_utime);
			}
			if (get_stime) {
				printf("stime=%u ", proc_info.proc_stime);
			}
			if (get_vmem) {
				printf("vmemory=%d ", proc_info.proc_virtual_mem_size);
			}
			if (get_cmdline) {
				printf("[%s] ", proc_info.cmdline);
			}
			printf("\n");

			head = head->next;
		}
	}
	else {
		if (isUserProcess(proc_id)) {
			stat_statm_cmdline_fields proc_info = stat_statm_cmdline_parser(proc_id);
				printf("%d: ", proc_id);
			if (get_state) {
				printf("%c ", proc_info.proc_state);
			}
			if (get_utime) {
				printf("utime=%u ", proc_info.proc_utime);
			}
			if (get_stime) {
				printf("stime=%u ", proc_info.proc_stime);
			}
			if (get_vmem) {
				printf("vmemory=%d ", proc_info.proc_virtual_mem_size);
			}
			if (get_cmdline) {
				printf("[%s] ", proc_info.cmdline);
			}
			printf("\n");
		}
	}
}
