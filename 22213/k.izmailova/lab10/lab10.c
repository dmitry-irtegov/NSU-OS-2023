#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc<2){
        exit(1);
    }
    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork failed\n");
            exit(1);
        case 0:
            if (execvp(argv[1], argv + 1) == -1) {
                perror("exec fail\n");
                exit(1);
            }
        default:{
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed\n");
                exit(1);
            } else {
                printf("Child has finished with status %d\n", status);
            }
           }
    }

    return 0;
}