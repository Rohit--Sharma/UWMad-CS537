#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	extern int optopt;

	int get_all_proc_ids = 1;
	int proc_id = 0;
	int get_state = 0;
	int get_utime = 1;
	int get_stime = 0;
	int get_vmem = 0;
	int get_cmdline = 1;

	// opterr = 0;
	int c;

	while ((c = getopt(argc, argv, "p:sUSvc")) != -1)
		switch (c)
		{
			case 'p':
				get_all_proc_ids = 0;
				proc_id = atoi(optarg);
				break;
			case 's':
				get_state = 1;
				break;
			case 'U':
				get_utime = 0;
				break;
			case 'S':
				get_stime = 1;
				break;
			case 'v':
				get_vmem = 1;
				break;
			case 'c':
				get_cmdline = 0;
				break;
			case '?':
				if (optopt == 'p')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}

	printf("Get all proc ids: %d\nProc id: %d\nGet state: %d\nGet Utime: %d\nGet stime: %d\nGet vmem: %d\nGet cmdline: %d\n", get_all_proc_ids, proc_id, get_state, get_utime, get_stime, get_vmem, get_cmdline);

	int index;
	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);

	return 0;
}
