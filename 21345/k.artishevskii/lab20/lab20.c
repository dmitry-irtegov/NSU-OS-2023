#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>
#include <stdbool.h>
#include <sys/stat.h>


bool is_find_file_in_all_folder = false;


int isDir(const char* path){
	struct stat pathStat;
	if(stat(path, &pathStat) != 0){
		return 0;
	}
	return S_ISDIR(pathStat.st_mode);
}


static void lookup(const char *pattern, const char* folder){
	DIR *dirp;
	struct dirent *dp;

	if ((dirp = opendir(folder)) == NULL) {
		fprintf(stderr, "Couldn't open %s\n", folder);
		return;
	}

	do{
		errno = 0;
		if((dp = readdir(dirp)) != NULL){
			char *fullpath = NULL;
			size_t pathLen = strlen(folder) + strlen(dp->d_name) + 2;
			fullpath = (char *)malloc(pathLen * sizeof(char));
			if (fullpath == NULL) {
				perror("Memory allocation failed");
				closedir(dirp);
				return;
			}
			snprintf(fullpath, pathLen, "%s/%s", folder, dp->d_name);

			if (strncmp(fullpath, "./", 2) == 0) {
				memmove(fullpath, fullpath + 2, strlen(fullpath));
			}

			if (fnmatch(pattern, fullpath, FNM_PATHNAME) == 0 && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
				printf("Found file: %s \n", fullpath);
				is_find_file_in_all_folder = true;
			}
			if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strstr(pattern, "/") != NULL && isDir(fullpath)) {
				lookup(pattern, fullpath);
			}
			free(fullpath);
		}
	}
	while (dp != NULL);
	if (errno != 0){
		perror("Error while searching for template");
	}
	if(closedir(dirp) == -1){
		perror("Error closedir");
	}
	return;
}

int main(int argc, char *argv[]){
	for (int i = 1; i < argc; i++){
		is_find_file_in_all_folder = false;
		lookup(argv[i], ".");
		if(!is_find_file_in_all_folder){
			printf("Failed to find pattern %s\n", argv[i]);
		}
	}
	return 0;
}
