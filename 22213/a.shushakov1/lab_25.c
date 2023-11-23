#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#define MSGSIZE 20

int main(int argc, char* argv[]) {
    int fildes[2];

    if (pipe(fildes) == -1) {
        perror(argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    switch(pid){
       
        case(-1):
            perror("Cannot fork\n");
            exit(1);
            break;
       
        case(0):
            close(fildes[0]);
            char msgout[] = "I'm tIrEd";
            if (write(fildes[1], msgout, MSGSIZE) == -1) {
                perror("Error: Child write\n");
            }
            close(fildes[1]);
            break;
       
        default:
            close(fildes[1]);
            char msgin[MSGSIZE];
            if (read(fildes[0], msgin, MSGSIZE) == -1) {
                perror("Error: Parent read\n");
            }
            close(fildes[0]);

            for (int i = 0; i < MSGSIZE - 1; i++) {
                msgin[i] = toupper(msgin[i]);
            }
            printf("Got: %s\n", msgin);
            break;
    }
    exit(0);
}
