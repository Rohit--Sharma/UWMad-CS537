#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

main() {
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir("/proc")) == NULL)
		perror("opendir() error");
	else {
		puts("contents of root:");
    		while ((entry = readdir(dir)) != NULL)
      			printf("  %s\n", entry->d_name);
    		closedir(dir);
	}
}

