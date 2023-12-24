#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXSIZE 2048

int main() {
    FILE* input = popen("printf '%b\n' 'some text'", "r");
    if (!input) {
        perror("popen failed");
        exit(1);
    }

    FILE* output = popen("tr [:lower:] [:upper:]", "w");
    if (!output) {
        perror("popen failed");
        exit(1);
    }

    char buff[MAXSIZE];
    while (fgets(buff, MAXSIZE, input)) {
        fprintf(output, "%s", buff);
    }

    if (pclose(input) < 0) {
        perror("error in input child process");
        exit(1);
    }

    if (pclose(output) < 0) {
        perror("error in output child process");
        exit(1);
    }

    return 0;
}
