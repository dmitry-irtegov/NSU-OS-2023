#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {

    pid_t pid = fork();
    if (pid == -1) {
         return -1;
    }

    if (pid == 0) {
        execlp("cat", "cat", "task2.c", NULL);
        return 0;
    }
    else {
        if(wait(NULL) != -1) {
            printf("Child process has finished\n");
        }
    }

    return 0;
}

