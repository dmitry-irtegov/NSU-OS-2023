#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    pid_t child = fork();
    if (child == -1) {
        perror("exec fork failed");
    }
    if (child == 0) {
        execlp("cat", "cat", "text4.txt", NULL);
    }
    if (wait(0) > 0) {
        printf("Parent process\n");
    }
    else {
        perror("Not final child process");
    }
    printf("Parent program ending\n");
    return 0;
}