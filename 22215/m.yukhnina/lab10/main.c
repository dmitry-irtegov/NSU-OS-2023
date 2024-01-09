#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("No command!");
        return 1;
    }

    pid_t childID = fork();

    if (childID == -1) {
        perror("Bad fork!");
        return 1;
    }
    else if (childID == 0) {
        execvp(argv[1], argv + 1);
    }

    else {
        int status;
        wait(&status);
        printf("Child stat: %d\n", status);
    }

    return 0;
}