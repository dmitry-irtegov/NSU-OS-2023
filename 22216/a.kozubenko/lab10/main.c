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
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        fprintf(stdout, "Child PID: %d\n", getpid());
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            fprintf(stderr, "fork ended with code: %d\n", WEXITSTATUS(status));
        } else {
            fprintf(stderr, "fork ended not by exit. signal: %d\n", WTERMSIG(status));
        }
    }

    exit(EXIT_SUCCESS);
}

