#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUF_LEN 20

int main() {
    int fd[2];
    pid_t pid;
    char string[] = "time slips away, burn out - don't fade\n";

    if (pipe(fd) == -1) {
        perror("pipe() caused an error ");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    switch(pid) 
    {
    case -1:
        close(fd[0]);
        close(fd[1]);
        perror("fork() caused an error ");
        exit(EXIT_FAILURE);
    case 0:
        close(fd[0]);
        write(fd[1], string, strlen(string));
        close(fd[1]);
        exit(EXIT_SUCCESS);

    default:
        close(fd[1]);
        ssize_t bytesRead;
        char buf[BUF_LEN];
        while ((bytesRead = read(fd[0], buf, BUF_LEN)) > 0) {
            for (int i = 0; i < bytesRead; ++i) {
                putchar(toupper(buf[i]));
            }
        }
        close(fd[0]);
        if (bytesRead < 0) {
            perror("parent's read() caused an error ");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }    

}