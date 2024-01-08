#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fnmatch.h>

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

int main () {
    DIR *dir;
    struct dirent *entry;
    int size = sysconf(_SC_LINE_MAX);
    char pattern[size];
    
    printf("Write your pattern:\n");
    scanf("%s", pattern);

    dir = opendir("./");
    if (!dir) {
        perror("Dir open failed");
        return -1;
    }

    int amount = 0;
    entry = readdir(dir);
    while (entry != NULL) {
        // можно использовать fnmatch
        if(p_match(entry->d_name, pattern)){
            printf("%s\n",entry->d_name);
            amount++;
        }
	    entry = readdir(dir);
    }

    if (!amount){
        printf("No files with such pattern: %s\n", pattern);
    }
        
    closedir(dir);
    return 0;
}
