#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough args");
        return -1;
    }

    char line[BUFSIZ];

    char cmd[BUFSIZ];
    if (snprintf(cmd, sizeof(cmd), "cat %s", argv[1]) < 0) {
        perror("Error with snprintf");
        return -1;
    }

    FILE* in = popen(cmd, "r");
    if (in == NULL) {
        perror("Error with popen");
        return -1;
    }

    FILE* out = popen("tr [:lower:] [:upper:]", "w");
    if (out == NULL) {
        perror("Error with popen");
        return -1;
    }

    while (fgets(line, BUFSIZ, in) != NULL) {
        if (fputs(line, out) == -1) {
            perror("Error with fputs");
            return -1;
        }
    }

    if (pclose(in) == -1) {
        perror("Error with pclose");
        return -1;
    }

    if (pclose(out) == -1) {
        perror("Error with pclose");
        return -1;
    }

    return 0;
}
