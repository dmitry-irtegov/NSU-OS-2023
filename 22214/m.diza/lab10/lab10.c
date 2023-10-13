#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[ ]) {
    
    pid_t pid;

    switch (pid = fork()) {
        case -1: 
            perror("fork() error");
            exit(1);
        case 0:     //child
            if (execv(argv[1], &argv[1]) == -1) 
                perror("exec error");
        default:
            int return_value;
            if (wait(&return_value) == -1) 
                perror("wait() error");
            else
                if(WIFEXITED(return_value)) {
                    printf("returned value: %d", WEXITSTATUS(return_value));
                }
                else {
                    fprintf(stderr, "The process was interrupted by a signal");
                }
    }
    return 0;
}   