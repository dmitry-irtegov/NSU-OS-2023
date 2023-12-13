#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int execvpe(const char *file, char *const argv[], char *envp[]);

int main(int argc, char* argv[]) {
    char* argv1[] = {"test11", NULL};
    char* envp[] = {"TZ=America/New_York", NULL};

    pid_t pid;
    int status;

    switch (pid = fork()) {
        case -1:
            perror("Fork cannot create new subprocess\n");
            exit(EXIT_FAILURE);
        case 0:
            if (execvpe("test11", argv1, envp) == -1) {
                perror("Error with executing your args");
                exit(EXIT_FAILURE);
            }

        default:
            if (wait(&status) == -1) {
                perror("Cannot wait for subprocess");
                exit(EXIT_FAILURE);
            }
            printf("Child exit status - %d\n", WEXITSTATUS(status));
}

    exit(EXIT_SUCCESS);
}

int execvpe(const char *file, char *const argv[], char *envp[]) {
    environ = envp;
    return execvp(file, argv);
}