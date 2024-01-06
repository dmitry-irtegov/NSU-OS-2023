#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

extern char** environ;

int my_execvpe(const char* filename, char** argv, char** envp)
{
    char** tmp = environ;
    environ = envp;
    execvp(filename, argv);
    environ = tmp;
    return -1;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Enter the command after %s\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid > 0)
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            fprintf(stdout, "\nchild's exit status is: %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            fprintf(stdout, "\nsignal is: %d\n", WTERMSIG(status));
        }
    }
    else
    {
        char* new_environ[] = { "PATH=/usr/bin", "MY_VALUE=value", "USER=me", "TZ=Asia/Moscow", "HOME=/home/students/20300", (char*)0 };
        if (my_execvpe(argv[1], &argv[1], new_environ) == -1) {
            perror("execvpe");
            exit(1);
        }
    }
    return 0;
}
