#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        close(fd[0]);
        char str[15] = "some text\n";
        if (write(fd[1], str, sizeof(str)) < 0) {
            perror("write failed");
            exit(1);
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        char c;
        ssize_t k = 1;
        while (k != 0) {
            k = read(fd[0], &c, 1);
            if (k < 0) {
                perror("read failed");
                exit(1);
            }
            putchar(toupper(c));
        }
        close(fd[0]);
    }
    return 0;
}