#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 128

int main(int argc, char *argv[]) {
    FILE *out;
    char result[SIZE];
    int exit_status;

    out = popen("grep '^$' file.txt | wc -l", "r");

    fgets(result, sizeof(result), out);

    exit_status = pclose(out);
    if (exit_status != 0) {
        fprintf(stderr, "an error was occured\n");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(exit_status)) {
        fprintf(stderr, "command finished with code '%d'\n", WEXITSTATUS(exit_status));
    }

    printf("result: %s\n", result);
    exit(EXIT_SUCCESS);
}

