#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Using: %s <command>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* pipe = popen(argv[1], "r");

    if (!pipe) {
        perror("popen");
        return EXIT_FAILURE;
    }

    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, pipe) != NULL) {
        for (int i = 0; buffer[i] != '\0'; ++i) {
            buffer[i] = toupper(buffer[i]);
        }

        fprintf(stdout,"%s", buffer);
    }

    if (pclose(pipe) == -1) {
        perror("pclose");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
