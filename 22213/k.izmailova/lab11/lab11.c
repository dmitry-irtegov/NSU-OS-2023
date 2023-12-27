#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

extern char** environ;

int execvpe(char* filename, char* arg[], char* envp[]) {
    char** ptr;
    environ = envp;

    printf("New environ:\n");
    for (ptr = environ; *ptr != NULL; ptr++) {
        printf("%s\n", *ptr);
    }

    execvp(filename, arg);
    return -1;
}

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid == -1) { return 1; }

    if (pid == 0) {
        // Child process
        char* args[] = { "date", NULL };
        execvpe("date", args, argv + 1);
        return 1;
    }
    else {
        // Parent process
        if (wait(NULL) != -1) {
            printf("\nChild process (pid: %d) finished\n", pid);
        }
    }

    return 0;
}
