#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MSGSIZE 100

int main(int argc, char** argv) {
    int fileDescriptor[2];
    pid_t childPID1, childPID2;
    char buffer[MSGSIZE];
    int fileDescriptorInput;

    if (argc == 2) {
        fileDescriptorInput = open(argv[1], O_RDONLY);
    } else {
        fprintf(stderr, "Incorrect number of arguments\n");
        return 1;
    }

    if (pipe(fileDescriptor) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    childPID1 = fork();

    if (childPID1 == -1) {
        perror("Cannot create fork1");
        close(fileDescriptor[0]);
        close(fileDescriptor[1]);
        _exit(EXIT_FAILURE);
    }

    if (childPID1 == 0) {
        close(fileDescriptor[0]);
        int length;
        while ((length = read(fileDescriptorInput, buffer, MSGSIZE)) > 0) {
            write(fileDescriptor[1], buffer, length);
        }
        close(fileDescriptor[1]);
        return 0;
    }

    childPID2 = fork();

    if (childPID2 == -1) {
        perror("Cannot create fork2");
        close(fileDescriptor[0]);
        close(fileDescriptor[1]);
        _exit(EXIT_FAILURE);
    }

    if (childPID2 == 0) {
        close(fileDescriptor[1]);
        int length;
        while ((length = read(fileDescriptor[0], buffer, MSGSIZE))) {
            for (int i = 0; i < length; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            write(1, buffer, length);
        }
        close(fileDescriptor[0]);
        return 0;
    }

    close(fileDescriptor[0]);
    close(fileDescriptor[1]);

    while (wait(NULL) != -1);

    return 0;
}
