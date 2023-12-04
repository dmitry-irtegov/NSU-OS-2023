#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [argumentss]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("Failed toexecvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Process completion code: %d\n", WEXITSTATUS(status));     
        } else {
            printf("The process ended not by exit\n");
        }
    }

    exit(EXIT_SUCCESS);
}

