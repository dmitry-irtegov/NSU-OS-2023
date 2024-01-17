#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define BUF_LEN 15

int main(int argc, char* argv[]) {
    int fd[2];
    pid_t pid;

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
        int textfd;
        if ((textfd = open(argv[1], O_RDONLY)) == -1) {
            perror("child could not open the file");
            exit(EXIT_FAILURE);
        }
        ssize_t bRead;    
        char string[BUF_LEN];

        while ((bRead = read(textfd, string, BUF_LEN)) > 0) {
            if (write(fd[1], string, bRead) == -1) {
                perror("writing from child caused an error ");
                exit(EXIT_FAILURE);
            }
        }
        if (bRead < 0) {
            perror("child's read() caused an error ");
            exit(EXIT_FAILURE);
        }

        close(fd[1]);
        close(textfd);
        exit(EXIT_SUCCESS);

    default:
        close(fd[1]);
        ssize_t bytesRead;
        char buf[BUF_LEN];
        while ((bytesRead = read(fd[0], buf, BUF_LEN)) > 0) {
            for (int i = 0; i < bytesRead; ++i) {
                buf[i] = toupper(buf[i]);
            }
            if (write(STDOUT_FILENO, buf, bytesRead) == -1) {
                perror("writing from parent caused an error ");
                exit(EXIT_FAILURE);
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