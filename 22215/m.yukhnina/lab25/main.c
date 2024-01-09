#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define BUF_SIZE 20


int main() {
    int descriptors[2];
    if (pipe(descriptors) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid == -1){
        perror("fork");
        close(descriptors[0]);
        close(descriptors[1]);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(descriptors[1]); // closing the write end.
        char buf[BUF_SIZE];

        if (read(descriptors[0], buf, BUF_SIZE) == -1)
        {
            close(descriptors[0]);
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(descriptors[0]);

        for (int i = 0; i < BUF_SIZE; i++)
        {
            printf("%c", toupper(buf[i]));
        }
        printf("\n");
    }
    else {                 // parent process
        close(descriptors[0]); // closing the read end.
        char *message = "sBBBBsBBsBBBs";
        if (write(descriptors[1], message, BUF_SIZE) == -1) {
            perror("write");
            close(descriptors[1]);
            exit(EXIT_FAILURE);
        }
        close(descriptors[1]);

        if (waitpid(pid, NULL, 0) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
