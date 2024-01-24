#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fnmatch.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Argc error.");
		return -1;
	}
	if (strchr(argv[1], '/') != NULL) {
		fprintf(stderr, "Pattern contains '/'.");
		return -1;
	}
	DIR* dir;
	if (!(dir = opendir("."))) {
		perror("Opendir failed.");
		return -1;
	}
	struct dirent* entry;
	int count = 0;
	errno = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (fnmatch(argv[1], entry->d_name, FNM_PATHNAME) == 0) {
			printf("%s\n", entry->d_name);
			count++;
		}
	}
	if (errno != 0) {
		perror("Readdir failed.");
		closedir(dir);
		return -1;
	}
	if (count == 0) {
		printf("%s", argv[1]);
	}
	closedir(dir);
	return 0;
}
