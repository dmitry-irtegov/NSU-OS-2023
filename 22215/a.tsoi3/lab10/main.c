#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("No command to execute!");
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0) {  // Error case
        perror("Couldn't make a subprocess");
        return -1;
    }
    else if (pid == 0) {  // Code for subprocess
        execvp(argv[1], argv + 1);
    }
    else { // Code for current process
        int status;
        wait(&status);
        printf("Command EXIT STATUS: %d\n", status);
    }

    return 0;
}

