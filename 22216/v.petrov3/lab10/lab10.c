#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
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
        } else {
            printf("%d\n", status);
        }
        printf("Вывод родительского процесса...\n");
    } 
    else {
        printf("Вывод дочернего процесса...\n");
        if (argc > 1) {
            execvp(argv[1], argv + 1);
            perror("execvp failed");
            exit(1);
        }
    }
    
    return 0;
}
