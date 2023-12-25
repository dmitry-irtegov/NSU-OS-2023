#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    pid_t pid = fork(); /* если fork завершился успешно, pid > 0 в родительском процессе */
    if (pid < 0) {
        perror("make new process");
    /* fork потерпел неудачу */
    } else if (pid > 0) {
    /* здесь располагается родительский код */
        int status;
        if (waitpid(-1, &status, 0) == -1) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                printf("\nChild exited normally with code %d\n", WEXITSTATUS(status));
            } else {
                perror("Child process error\n");
            }
        }
        printf("Вывод родительского процесса...\n");
    } else {
    /* здесь располагается дочерний код */
        printf("Вывод дочернего процесса...\n");
        execl("/usr/bin/cat", "cat", "test.txt", (char*)NULL);
        perror("exec skript");
    }
    
    return 0;
}
