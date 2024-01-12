#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid > 0) {
        int status;
        if (waitpid(-1, &status, 0) == -1) {
            perror("waitpid");
            exit(1);
        } 
        else {
            if (WIFEXITED(status)) {
                printf("\nChild exited normally with code %d\n", WEXITSTATUS(status));
            } else {
                perror("Child process error\n");
                exit(1);
            }
        }
        printf("Вывод родительского процесса...\n");
    } 
    else {
        printf("Вывод дочернего процесса...\n");
        execl("/usr/bin/cat", "cat", "test.txt", (char*)NULL);
        perror("execl failed");
    }
    
    return 0;
}
