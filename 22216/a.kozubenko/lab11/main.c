#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int execvpe(const char *file, char *const argv[], char *const envp[]) {
    if (envp != NULL) {
        environ = envp;
    }

    return execvp(file, argv);
}

int main() {
    char *argv[] = {"ls", "-l", NULL};
    char *env[] = {"PATH=/home/students/22200/a.kozubenko", NULL};

     if (execvpe("ls", argv, env) == -1) {
        perror("execvpe error");
        exit(1);
    }

    exit(0);
}

