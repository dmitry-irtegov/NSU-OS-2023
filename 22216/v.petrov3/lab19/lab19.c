#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fnmatch.h>
#include <errno.h>

int p_match(char *filename, char *pattern) {
    int i = 0;
    int j = 0;
    while (filename[i] != '\0' && pattern[j] != '\0') {
        if (pattern[j] == '[' || pattern[j] == ']') {
            j ++;
            continue;
        }
        if (pattern[j] != filename[i] && pattern[j] != '?'){
            if (pattern[j] == '*') {
                while(pattern[j] == '*') {j++;}
                if (pattern[j] == '\0') {
                    return 1;
                }
                else {
                    while (filename[i] != pattern[j]) {
                        i++;
                    }
                }
            }
            else {
                return 0;
            }
        }
        i++;
        j++;
    }
    return pattern[j] == '\0' && filename[i] == '\0';
}

int main (int argc, char**argv) {
    DIR *dir;
    struct dirent *entry;
    if (argc != 2) {
        fprintf(stderr, "Usage: ./a.out pattern\n");
        exit(0);
    }

    dir = opendir(".");
    if (!dir) {
        perror("Dir open failed");
        exit(1);
    }

    int amount = 0;
    entry = readdir(dir);
    while (entry != NULL) {
        if(p_match(entry->d_name, argv[1])){
            printf("%s\n",entry->d_name);
            amount++;
        }
	entry = readdir(dir);
    }

    if (errno) {
        perror("readdir failed");
        exit(1);
    }

    if (!amount){
        printf("No files with such pattern: %s\n", argv[1]);
    }
        
    closedir(dir);
    return 0;
}
