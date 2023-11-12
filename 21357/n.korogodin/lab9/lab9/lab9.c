#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t p = fork();
    if (p < 0)
    {
        perror("fork fail");
        exit(1);
    }
    else if (p == 0)
    {
        printf("%d : It's child process\n", getpid());
        FILE* file = fopen("lab9file.txt", "r");
        if (file == NULL)
        {
            perror("open file failed");
        }
        char display;
        printf("File includes: ");
        while ((display = fgetc(file)) != EOF)
        {
            printf("%c", display);
        }
        printf("\n");
    }
    else
    {
        printf("It's start of parent process\n");
        wait(0);
        printf("%d : ", getpid());
        printf("It's end of parent process\n");
    }
}
