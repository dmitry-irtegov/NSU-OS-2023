#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    

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
            printf("%d\n", status);
        }
        printf("Вывод родительского процесса...\n");
    } else {
    /* здесь располагается дочерний код */
        printf("Запуск дочернего процесса...\n");
        execvp(argv[1], argv + 1);
        perror("exec cmd");
    }
    
    return 0;
}
