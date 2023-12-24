#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: lab27 filename\n");
        exit(0);
    }
    char command[1024];
    char result[100];
    int exit_status;

    if (sprintf(command, "grep -c ^$ %s", argv[1]) < 0) {
        perror("sprintf failed");
        exit(1);
    }

    FILE *command_output = popen(command, "r");
    if (!command_output) {
        perror("popen failed");
        exit(1);
    }

    fgets(result, sizeof(result), command_output);

    exit_status = pclose(command_output);
    if (exit_status != 0) {
        perror("child failed");
        exit(1);
    }

    printf("result: %s", result);
    return 0;
}
