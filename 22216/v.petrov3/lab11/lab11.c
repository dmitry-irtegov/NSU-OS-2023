#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char** environ;

int execvpe(const char* filename, char** argv, char** envp) {
    char **safe_environ = environ;
    environ = envp;
    execvp(filename, argv);
    environ = safe_environ;
    return -1;
}

int main(int argc, char** argv) {
    char *envp[] = {"HOME=/Users/vovapetrov", "PATH=/usr/bin/", NULL};
    
    if (argc >= 2) {
        if (execvpe(argv[1], argv + 1, envp) == -1) {
            perror("execvpe failed");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Usage: /lab11 <command> <args>\n");
    }
    return 0;
}
