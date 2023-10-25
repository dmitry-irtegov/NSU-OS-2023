#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
    FILE *fpin;
    char line[BUFSIZE];
    int emptyLineCount = 0;

    if (argc < 2) {
        printf("Not enough arguments\n");
        exit(1);
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
        exit(1);
    }

    while (fgets(line, BUFSIZE, fpin) != NULL) {
        if (line[0] == '\n') {
            emptyLineCount++;
        }
    }

    fclose(fpin);

    printf("Empty lines: %d\n", emptyLineCount);

    return 0;
}

